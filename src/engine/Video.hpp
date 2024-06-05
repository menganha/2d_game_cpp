#pragma once

#include "Texture.hpp"

#include <SDL2/SDL.h>
#include <atomic>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <condition_variable>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

static constexpr int MAX_ERR_STR = 128;
static constexpr int MAX_VIDEOQ_SIZE = 64;

class Video
{
public:
    Video(std::string_view file_name, Texture& texture, SDL_Renderer* renderer);
    ~Video();
    void     UpdateTexture();     // Updates the created texture with frame from the queue if present
    void     StartDecodeThread(); // Starts a decoding thread and updates frame queue
    void     StopDecodeThread();
    Texture& GetTexture() const { return m_texture; }

private:
    std::string             m_file_name;
    int                     m_video_stream_index;
    AVFormatContext*        m_format_ctx;
    AVCodecContext*         m_codec_ctx;
    AVPacket*               m_packet;
    char                    m_error_str_buffer[MAX_ERR_STR];
    std::thread             m_decode_thread;
    std::mutex              m_mutex_queue;
    std::condition_variable m_cond_var;
    std::queue<AVFrame*>    m_queue_frames;
    std::atomic_bool        m_stop_decoding;
    Texture&                m_texture; // Not owned, just a reference
    SwsContext*             m_sws_ctx;
    AVFrame*                m_frame;   // Frame that would contain the final transformed and "scaled image"

    void DecodeVideoStream();
    void SetTexture(int width, int height, SDL_Renderer* renderer);
};
