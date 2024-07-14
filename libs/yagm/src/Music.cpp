#include "Music.hpp"

#include "../engine/Log.hpp"

#include <stdexcept>

Music::Music(const std::string_view music_filepath)
  : m_music{Mix_LoadMUS(music_filepath.data())}
{
  if (!m_music) {
    LERROR("Could not found requested music file %s: %s", music_filepath.data(), Mix_GetError());
    throw std::runtime_error(Mix_GetError());
  }
}

Music::~Music() { Mix_FreeMusic(m_music); }

void
Music::Play(int loop)
{
  Mix_PlayMusic(m_music, loop);
}

void
Music::Resume()
{
  Mix_ResumeMusic();
}

void
Music::Pause()
{
  Mix_PauseMusic();
}

