#pragma once

#include "Texture.hpp"

#include <SDL2/SDL.h>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

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
    static constexpr int MAX_ERR_STR = 128;
    static constexpr int MAX_VIDEOQ_SIZE = 1024;
    std::string          m_file_name;
    int                  m_video_stream_index;
    AVFormatContext*     m_format_ctx;
    AVCodecContext*      m_codec_ctx;
    AVPacket*            m_packet;
    char                 m_error_str_buffer[MAX_ERR_STR];
    std::thread          m_decode_thread;
    std::mutex           m_queue_mutex;
    std::queue<AVFrame*> m_queue_frames;
    bool                 m_stop_decoding; // TODO: Should we change it to an atomic?????
    Texture&             m_texture;       // Not owned, just a reference
    SwsContext*          m_sws_ctx;
    AVFrame*             m_frame;         // Frame that would contain the final transformed and "scaled image"

    void DecodeVideoStream();
    void SetTexture(int width, int height, SDL_Renderer* renderer);
};
