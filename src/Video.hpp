#pragma once

#include <SDL2/SDL.h>
#include <string>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class Video
{
public:
    Video(std::string_view file_name, SDL_Renderer* renderer);
    ~Video();
    void         Update();
    SDL_Texture* GetTexture() { return m_texture; }

private:
    static constexpr int MAX_ERR_STR = 128;
    std::string          m_file_name;
    int                  m_video_stream_index;
    SDL_Texture*         m_texture;
    AVFormatContext*     m_format_ctx;
    AVCodecContext*      m_codec_ctx;
    AVPacket*            m_packet;
    AVFrame*             m_frame;
    char                 m_error_str_buffer[MAX_ERR_STR];
};
