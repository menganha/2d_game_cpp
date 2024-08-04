#include "Video.hpp"

#include "Log.hpp"

extern "C"
{
#include <libavutil/imgutils.h>
}

Video::~Video()
{
  Video::StopDecodeThread();
  avcodec_free_context(&m_codec_ctx);
  avformat_close_input(&m_format_ctx);
  av_packet_free(&m_packet);
  sws_freeContext(m_sws_ctx);
  av_freep(&m_frame_scaled->data[0]); // Frees buffer that was allocated with av_image_alloc
  av_frame_free(&m_frame_scaled);
  if (m_texture)
    SDL_DestroyTexture(m_texture);
}

Video::Video(std::string_view file_name, SDL_Renderer* renderer)
  : m_file_name{file_name}
  , m_video_stream_index{-1}
  , m_format_ctx{nullptr}
  , m_codec_ctx{nullptr}
  , m_packet{av_packet_alloc()}
  , m_sws_ctx{nullptr}
  , m_frame_scaled{av_frame_alloc()} // av_frame_alloc does not alloc the data buffers
  , m_error_str_buffer{}
  , m_decode_thread{}
  , m_mutex_queue{}
  , m_queue_frames{}
  , m_stop_decoding{false}
  , m_texture{nullptr}
{
  if (!m_packet) {
    LERROR("Faild to allocate memory for AVPacket");
    return;
  }

  // Open and read Headers
  if (int retcode = avformat_open_input(&m_format_ctx, m_file_name.c_str(), nullptr, nullptr); retcode < 0) {
    LERROR(
      "Could not open video on %s: %s", m_file_name.c_str(), av_make_error_string(m_error_str_buffer, MAX_ERR_STR, retcode));
    return;
  }

  // Find Streams info
  if (int retcode = avformat_find_stream_info(m_format_ctx, nullptr); retcode < 0) {
    LERROR("Video error: %s", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, retcode));
    return;
  }

  for (auto idx = 0U; idx < m_format_ctx->nb_streams; idx++) {
    if (m_format_ctx->streams[idx]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && m_video_stream_index < 0)
      m_video_stream_index = idx; // Takes the first video stream
  }

  if (m_video_stream_index == -1) {
    LERROR("Could not find video stream");
    return;
  }

#ifndef NDEBUG
  av_dump_format(m_format_ctx, m_video_stream_index, m_file_name.c_str(), 0);
#endif

  // Find Decoder
  const AVCodec* codec;
  codec = avcodec_find_decoder(m_format_ctx->streams[m_video_stream_index]->codecpar->codec_id);
  if (codec == nullptr) {
    LERROR("Could not find Decoder for stream");
    return;
  }

  // Allocate a new codec context for the codec
  m_codec_ctx = avcodec_alloc_context3(codec);

  // Copy the parameters of the stream to the codec context
  if (int retcode = avcodec_parameters_to_context(m_codec_ctx, m_format_ctx->streams[m_video_stream_index]->codecpar);
      retcode < 0) {
    LERROR("Video error %s", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, retcode));
    return;
  }

  // Initialize the AVCodecContext to use the given AVCodec
  if (int retcode = avcodec_open2(m_codec_ctx, codec, nullptr); retcode < 0) {
    LERROR("Video error %s", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, retcode));
    return;
  }

  // Set the default texture
  SetTexture(m_codec_ctx->width, m_codec_ctx->height, renderer);
}

void
Video::StartDecodeThread(int loop)
{
  m_decode_thread = std::thread(&Video::DecodeVideoStream, this, loop);
}

void
Video::StopDecodeThread()
{
  m_stop_decoding.store(true); // The decoding thread might not stop immediately and would need more than
                               // one loop (2, 3 more, not sure... ???) to stop
  m_cond_var.notify_one();
  if (m_decode_thread.joinable())
    m_decode_thread.join();

  while (not m_queue_frames.empty()) {
    AVFrame* frame = m_queue_frames.front();
    av_frame_free(&frame);
    m_queue_frames.pop();
  }

  if (m_format_ctx) {
    int ret = avformat_seek_file(m_format_ctx, -1, INT64_MIN, m_format_ctx->start_time, m_format_ctx->start_time, 0);
    if (ret < 0)
      LWARN("Video warning: %s", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, ret));
  }

  if (m_codec_ctx)
    avcodec_flush_buffers(m_codec_ctx);

  m_stop_decoding.store(false);
}

void
Video::UpdateTexture()
{
  std::unique_lock<std::mutex> guard{m_mutex_queue};
  if (not m_queue_frames.empty()) {
    AVFrame* frame = m_queue_frames.front();
    m_queue_frames.pop();
    guard.unlock();
    m_cond_var.notify_one();

    sws_scale(
      m_sws_ctx, (const uint8_t* const*)frame->data, frame->linesize, 0, m_codec_ctx->height, m_frame_scaled->data,
      m_frame_scaled->linesize);

    av_frame_free(&frame);

    SDL_UpdateYUVTexture(
      m_texture, nullptr, m_frame_scaled->data[0], m_frame_scaled->linesize[0], m_frame_scaled->data[1],
      m_frame_scaled->linesize[1], m_frame_scaled->data[2], m_frame_scaled->linesize[2]);
  }
}

void
Video::DecodeVideoStream(int loop)
{

  while (true) {

    // Keep the value of the packet queue capped
    std::unique_lock<std::mutex> lock_cv{m_mutex_queue};
    m_cond_var.wait(lock_cv, [&]() { return ((m_queue_frames.size() < MAX_VIDEOQ_SIZE) or m_stop_decoding); });
    lock_cv.unlock();

    if (m_stop_decoding)
      break;

    if (!m_format_ctx)
      break;

    int ret{};
    ret = av_read_frame(m_format_ctx, m_packet);

    // Handle end-of-file
    if (ret == AVERROR_EOF) //
    {
      if (loop != 0) {
        loop--;
        LINFO("Looping video. loops left = %i", loop);
        ret = avformat_seek_file(m_format_ctx, -1, INT64_MIN, m_format_ctx->start_time, m_format_ctx->start_time, 0);
        if (ret < 0)
          LERROR("Video error: %s", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, ret));
        avcodec_flush_buffers(m_codec_ctx);
        av_packet_unref(m_packet);
        continue;
      }
      else {
        av_packet_unref(m_packet);
        break;
      }
    }
    else if (ret < 0) {
      LERROR("Video error: %s", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, ret));
      break;
    }

    // Manual switch to break from decoding usually set in the destructor

    // If it's the video stream, decode
    if (m_packet->stream_index == m_video_stream_index) {
      ret = avcodec_send_packet(m_codec_ctx, m_packet);

      if (ret < 0)
        LERROR("Video error: %s", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, ret));

      while (ret >= 0) {
        // TODO: We are reallocating a bunch, i.e., on each decoding iteration.
        // Consider having a fixed sized array for the frame queue with space allocated for all frames
        AVFrame* frame{av_frame_alloc()};
        if (!frame) {
          LERROR("Video error: %s", "Failed to allocate memory for AVFrame");
          break;
        }

        ret = avcodec_receive_frame(m_codec_ctx, frame);

        if (ret == AVERROR(EAGAIN) or ret == AVERROR_EOF < 0) {
          av_frame_free(&frame);
          break;
        }
        else if (ret < 0) {
          LERROR("Error receiving frame: %s", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, ret));
          break;
        }

        std::lock_guard<std::mutex> guard{m_mutex_queue};
        m_queue_frames.push(frame);
      }
    }
    av_packet_unref(m_packet);
  }
}

void
Video::SetTexture(int width, int height, SDL_Renderer* renderer)
{

  m_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, width, height);

  // Setup the data pointers and linesizes based on the specified image parameters and the provided array.
  if (int ret = av_image_alloc(m_frame_scaled->data, m_frame_scaled->linesize, width, height, AV_PIX_FMT_YUV420P, 32);
      ret < 0) {
    LERROR("Video error %s", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, ret));
    return;
  }

  // Scaling context
  sws_freeContext(m_sws_ctx);
  m_sws_ctx = sws_getContext(
    m_codec_ctx->width, m_codec_ctx->height, m_codec_ctx->pix_fmt, width, height, AV_PIX_FMT_YUV420P, SWS_BILINEAR,
    nullptr, nullptr, nullptr);
}
