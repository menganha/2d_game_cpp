#pragma once

#include "Font.hpp"
#include "Music.hpp"
#include "Video.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <filesystem>
#include <map>

//
// Handles PnG Textures, Fonts, Sounds and Music
// Id of file-type resources is simply the relative path
//
class AssetManager
{
public:
  AssetManager(const char* assets_dir, SDL_Renderer* renderer);

  // Font
  void        AddFont(const char* relative_path, int size, const char* alias = nullptr);
  const Font& GetFont(std::string_view relative_path) const;

  // Video
  void   AddVideo(const char* relative_path); // Adds also a texture with the same id
  Video& GetVideo(std::string_view relative_path);

  // Music and audio
  void   AddMusic(const char* relative_path);
  Music& GetMusic(std::string_view relative_path);

  // Texture. One can only "get" the raw texture, not add them. TODO: Consider removing them?
  SDL_Texture* GetTexture(std::string_view relative_path);

  std::filesystem::path GetAbsolutePath(const char* relative_path) const;
  std::string           GetAbsolutePathStr(const char* relative_path) const;
  std::string           GetAbsolutePathStr(std::string_view relative_path) const;

private:
  SDL_Renderer*                                    m_renderer;
  std::filesystem::path                            m_assets_dir;
  std::map<std::string, Font, std::less<>>         m_text_cache;
  std::map<std::string, Video, std::less<>>        m_video_cache;
  std::map<std::string, SDL_Texture*, std::less<>> m_texture_cache;
  std::map<std::string, Music, std::less<>>        m_music_cache;
};
