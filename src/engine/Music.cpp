#include "Music.hpp"

Music::Music(const std::string_view music_filepath)
  : m_music{Mix_LoadMUS(music_filepath.data())}
{
}

void
Music::Play(int loop)
{
  Mix_PlayMusic(m_music, loop);
}

void
Music::Pause()
{
  Mix_PauseMusic();
}

Music::~Music()
{
  Mix_FreeMusic(m_music);
}
