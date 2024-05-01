#include "Video.hpp"

#include <chrono>
#include <spdlog/spdlog.h>
#include <stdexcept>
extern "C" {
#include <libavutil/imgutils.h>
}

Video::~Video()
{
    Video::StopDecodeThread();
    avcodec_free_context(&m_codec_ctx);
    avformat_close_input(&m_format_ctx);
    av_packet_free(&m_packet);
    sws_freeContext(m_sws_ctx);
    av_freep(&m_frame->data[0]); // Frees buffer that was allocated with av_image_alloc
    av_frame_free(&m_frame);
}

Video::Video(std::string_view file_name, Texture& texture, SDL_Renderer* renderer)
    : m_file_name{file_name}
    , m_video_stream_index{-1}
    , m_format_ctx{nullptr}
    , m_codec_ctx{nullptr}
    , m_packet{av_packet_alloc()}
    , m_error_str_buffer{}
    , m_decode_thread{}
    , m_queue_mutex{}
    , m_queue_frames{}
    , m_stop_decoding{false}
    , m_texture{texture}
    , m_sws_ctx{nullptr}
{
    if (!m_packet)
        throw std::runtime_error("Faild to allocate memory for AVPacket");

    int return_code;

    // Open and read Headers
    return_code = avformat_open_input(&m_format_ctx, m_file_name.c_str(), nullptr, nullptr);
    if (return_code < 0)
        throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, return_code));

    // Find Streams info
    return_code = avformat_find_stream_info(m_format_ctx, nullptr);
    if (return_code < 0)
        throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, return_code));

    for (auto idx = 0U; idx < m_format_ctx->nb_streams; idx++)
    {
        if (m_format_ctx->streams[idx]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && m_video_stream_index < 0)
            m_video_stream_index = idx; // Takes the first video stream
    }
    if (m_video_stream_index == -1)
        throw std::runtime_error("Could not find video stream for " + m_file_name);

#ifndef NDEBUG
    av_dump_format(m_format_ctx, m_video_stream_index, m_file_name.c_str(), 0);
#endif

    // Find Decoder
    const AVCodec* codec;
    codec = avcodec_find_decoder(m_format_ctx->streams[m_video_stream_index]->codecpar->codec_id);
    if (codec == nullptr)
        throw std::runtime_error("Could not find Decoder for stream");

    // Allocate a new codec context for the codec
    m_codec_ctx = avcodec_alloc_context3(codec);

    // Copy the parameters of the stream to the codec context
    return_code = avcodec_parameters_to_context(m_codec_ctx, m_format_ctx->streams[m_video_stream_index]->codecpar);
    if (return_code < 0)
        throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, return_code));

    // Initialize the AVCodecContext to use the given AVCodec

    return_code = avcodec_open2(m_codec_ctx, codec, nullptr);
    if (return_code < 0)
        throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, return_code));

    // Set the default texture
    SetTexture(m_codec_ctx->width, m_codec_ctx->height, renderer);
}

void
Video::StartDecodeThread()
{
    m_decode_thread = std::thread(&Video::DecodeVideoStream, this);
}

void
Video::StopDecodeThread()
{
    m_stop_decoding = true; // The decoding thread might not stop immediately and would need more than
                            // one loop (2, 3 more, not sure... ???) to stop
    if (m_decode_thread.joinable())
        m_decode_thread.join();

    while (not m_queue_frames.empty())
    {
        AVFrame* frame = m_queue_frames.front();
        av_frame_unref(frame);
        av_frame_free(&frame);
        m_queue_frames.pop();
    }

    int ret = avformat_seek_file(m_format_ctx, -1, INT64_MIN, m_format_ctx->start_time, m_format_ctx->start_time, 0);
    if (ret < 0)
        throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, ret));
    avcodec_flush_buffers(m_codec_ctx);

    m_stop_decoding = false;
}

void
Video::UpdateTexture()
{
    std::unique_lock<std::mutex> guard{m_queue_mutex};
    if (!m_queue_frames.empty())
    {
        AVFrame* frame = m_queue_frames.front();
        m_queue_frames.pop();
        guard.unlock();

        sws_scale(m_sws_ctx,
                  (const uint8_t* const*)frame->data,
                  frame->linesize,
                  0,
                  m_codec_ctx->height,
                  m_frame->data,
                  m_frame->linesize);

        av_frame_unref(frame);
        av_frame_free(&frame);

        SDL_UpdateYUVTexture(m_texture.GetTexture(),
                             nullptr,
                             m_frame->data[0],
                             m_frame->linesize[0],
                             m_frame->data[1],
                             m_frame->linesize[1],
                             m_frame->data[2],
                             m_frame->linesize[2]);
    }
}

void
Video::DecodeVideoStream()
{
    int loop{2};
    while (true)
    {

        // Keep the value limiterd of the debug queue
        auto size = m_queue_frames.size();
        if (size > MAX_VIDEOQ_SIZE)
        {
            // spdlog::debug("queue size {}", size);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }

        int read_frame_error = av_read_frame(m_format_ctx, m_packet);

        // Handle end-of-file
        if (read_frame_error == AVERROR_EOF) //
        {
            if (loop > 0)
            {
                spdlog::info("Looping video. loop = {}", loop);
                spdlog::debug("read_frame_error {}", av_make_error_string(m_error_str_buffer, MAX_ERR_STR, read_frame_error));
                int ret =
                  avformat_seek_file(m_format_ctx, -1, INT64_MIN, m_format_ctx->start_time, m_format_ctx->start_time, 0);
                if (ret < 0)
                    throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, ret));
                avcodec_flush_buffers(m_codec_ctx);
                loop -= 1;
                av_packet_unref(m_packet);
                continue;
            }
            else
            {
                av_packet_unref(m_packet);
                break;
            }
        }
        else if (read_frame_error < 0)
        {
            throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, read_frame_error));
        }

        // Manual switch to break from decoding usually set in the destructor
        if (m_stop_decoding)
        {
            av_packet_unref(m_packet);
            break;
        }

        // If it's the video stream, decode
        if (m_packet->stream_index == m_video_stream_index)
        {
            int return_code = avcodec_send_packet(m_codec_ctx, m_packet);

            if (return_code < 0)
                throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, return_code));

            while (return_code >= 0)
            {

                AVFrame* frame{av_frame_alloc()};
                if (!frame)
                    throw std::runtime_error("Failed to allocate memory for AVFrame");

                return_code = avcodec_receive_frame(m_codec_ctx, frame);

                if (return_code == AVERROR(EAGAIN) or return_code == AVERROR_EOF)
                {
                    av_frame_unref(frame);
                    av_frame_free(&frame);
                    break;
                }
                else if (return_code < 0)
                    throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, return_code));

                std::lock_guard<std::mutex> guard{m_queue_mutex};
                m_queue_frames.push(frame);
            }
        }
        av_packet_unref(m_packet);
    }
}

void
Video::SetTexture(int width, int height, SDL_Renderer* renderer)
{

    m_texture = Texture{SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, width, height)};

    // Setup the data pointers and linesizes based on the specified image parameters and the provided array.
    m_frame = av_frame_alloc(); // av_frame_alloc does not alloc the data buffers
    int return_code = av_image_alloc(m_frame->data, m_frame->linesize, width, height, AV_PIX_FMT_YUV420P, 32);
    if (return_code < 0)
        throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, return_code));

    // Scaling context
    sws_freeContext(m_sws_ctx);
    m_sws_ctx = sws_getContext(m_codec_ctx->width,
                               m_codec_ctx->height,
                               m_codec_ctx->pix_fmt,
                               width,
                               height,
                               AV_PIX_FMT_YUV420P,
                               SWS_BILINEAR,
                               nullptr,
                               nullptr,
                               nullptr);
}
