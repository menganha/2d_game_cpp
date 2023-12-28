#include "Video.hpp"

#include <spdlog/spdlog.h>
#include <stdexcept>

Video::~Video()
{
    avcodec_free_context(&m_codec_ctx);
    avformat_close_input(&m_format_ctx); // Why two "frees" on m_format_ctx
    // avformat_free_context(m_format_ctx);
    av_frame_free(&m_frame);
    av_packet_free(&m_packet);
}

Video::Video(std::string_view file_name, SDL_Renderer* renderer)
    : m_file_name{file_name}
    , m_video_stream_index{-1}
    , m_texture{nullptr}
    , m_format_ctx{nullptr}
    , m_codec_ctx{nullptr}
    , m_packet{av_packet_alloc()}
    , m_frame{av_frame_alloc()}
    , m_error_str_buffer{}
{

    av_log_set_level(AV_LOG_TRACE);
    if (!m_packet)
        throw std::runtime_error("Faild to allocate memory for AVPacket");
    if (!m_frame)
        throw std::runtime_error("Failed to allocate memory for AVFrame");

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

    // ACTUAL DECODING PART
    AVFrame* p_frame = av_frame_alloc();
    if (p_frame == nullptr)
        throw std::runtime_error("Could not allocate frame AVFrame insatnce");

    // alloc an AVPacket to read data from the AVFormatContext
    AVPacket* p_packet = av_packet_alloc();
    if (p_packet == nullptr)
        throw std::runtime_error("Could not allocate frame AVPacket instance");

    // Create texture
    m_texture = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, m_codec_ctx->width, m_codec_ctx->height);

    // set up our SWSContext to convert the image data to YUV420
    // struct SwsContext* sws_ctx = nullptr;
    // sws_ctx = sws_getContext(m_codec_ctx->width,
    //                          m_codec_ctx->height,
    //                          m_codec_ctx->pix_fmt,
    //                          m_codec_ctx->width,
    //                          m_codec_ctx->height,
    //                          AV_PIX_FMT_YUV420P,
    //                          SWS_BILINEAR,
    //                          nullptr,
    //                          nullptr,
    //                          nullptr);
}

void
Video::Update()
{
    if (av_read_frame(m_format_ctx, m_packet) >= 0)
    {
        // if it's the video stream
        if (m_packet->stream_index == m_video_stream_index)
        {
            int return_code = avcodec_send_packet(m_codec_ctx, m_packet);

            if (return_code < 0)
            {
                throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, return_code));
            }

            while (return_code >= 0)
            {
                return_code = avcodec_receive_frame(m_codec_ctx, m_frame);
                if (return_code == AVERROR(EAGAIN) || return_code == AVERROR_EOF)
                {
                    break;
                }
                else if (return_code < 0)
                {
                    throw std::runtime_error(av_make_error_string(m_error_str_buffer, MAX_ERR_STR, return_code));
                }
                SDL_UpdateYUVTexture(m_texture,
                                     nullptr,
                                     m_frame->data[0],
                                     m_frame->linesize[0],
                                     m_frame->data[1],
                                     m_frame->linesize[1],
                                     m_frame->data[2],
                                     m_frame->linesize[2]);
                av_frame_unref(m_frame);
            }
        }
    }
    av_packet_unref(m_packet);
}
