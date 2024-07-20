#pragma once

#include <SDL2/SDL.h>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
extern "C"
{
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
  Video(std::string_view file_name, SDL_Renderer* renderer);
  ~Video();
  void         UpdateTexture();                 // Updates the created texture with frame from the queue if present
  void         StartDecodeThread(int loop = 0); // Starts a decoding thread and updates frame queue
  void         StopDecodeThread();
  SDL_Texture* GetTexturePtr() const { return m_texture; }

private:
  std::string             m_file_name;
  int                     m_video_stream_index;
  AVFormatContext*        m_format_ctx;
  AVCodecContext*         m_codec_ctx;
  AVPacket*               m_packet;
  SwsContext*             m_sws_ctx;
  AVFrame*                m_frame_scaled; // Frame that would contain the final transformed and "scaled image"
  std::queue<AVFrame*>    m_queue_frames;
  char                    m_error_str_buffer[MAX_ERR_STR];
  std::thread             m_decode_thread;
  std::mutex              m_mutex_queue;
  std::condition_variable m_cond_var;
  std::atomic_bool        m_stop_decoding;
  SDL_Texture*            m_texture;

  void DecodeVideoStream(int loop);
  void SetTexture(int width, int height, SDL_Renderer* renderer);
};
