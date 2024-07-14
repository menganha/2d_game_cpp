#pragma once

#include <SDL2/SDL_mixer.h>
#include <string_view>

class Music
{
public:
  Music(const std::string_view music_filepath);
  ~Music();
  void Play(int loop = 0);
  void Pause();
  void Resume();

private:
  Mix_Music* m_music;
};

