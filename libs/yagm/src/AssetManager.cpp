// Option 1: Add renderer to the class variables of asset manager. Con -> we have to live with this reference in this
// class (not beautiful) Option 2: Do some if/else whenever we're updating the texture. Con -> we have to check every
// frame for this function (not beautiful) Option 3: I don't know. 
// Best option so far is Option 1 and is the one implemented

#include "AssetManager.hpp"

#include <cassert>
#include <filesystem>

AssetManager::AssetManager(const char* assets_dir, SDL_Renderer* renderer)
  : m_assets_dir{std::filesystem::path(assets_dir).parent_path()}
  , m_renderer{renderer}
{
}

void
AssetManager::AddFont(const char* relative_path, int size, const char* alias)
{
  auto font_path = m_assets_dir / "assets" / relative_path;
  auto font_id = alias ? alias : relative_path;
  m_text_cache.try_emplace(font_id, font_path.c_str(), size, m_renderer);
  m_texture_cache.try_emplace(font_id, GetFont(font_id).GetTexturePtr());
}

const Font&
AssetManager::GetFont(std::string_view relative_path) const
{
  auto it = m_text_cache.find(relative_path);
  assert(it != m_text_cache.end());
  return it->second;
}

void
AssetManager::AddMusic(const char* relative_path)
{
  auto music_path = m_assets_dir / "assets" / relative_path;
  m_music_cache.try_emplace(relative_path, music_path.c_str());
}

Music&
AssetManager::GetMusic(std::string_view relative_path)
{
  auto it = m_music_cache.find(relative_path);
  assert(it != m_music_cache.end());
  return it->second;
}

void
AssetManager::AddVideo(const char* relative_path)
{
  auto video_path = m_assets_dir / "assets" / relative_path;
  m_video_cache.try_emplace(relative_path, video_path.c_str(), m_renderer);
  m_texture_cache.try_emplace(relative_path, GetVideo(relative_path).GetTexturePtr());
}

Video&
AssetManager::GetVideo(std::string_view relative_path)
{
  auto it = m_video_cache.find(relative_path);
  assert(it != m_video_cache.end());
  return it->second;
}

SDL_Texture*
AssetManager::GetTexture(std::string_view relative_path)
{
  auto it = m_texture_cache.find(relative_path);
  assert(it != m_texture_cache.end());
  return it->second;
}

std::filesystem::path
AssetManager::GetAbsolutePath(const char* relative_path) const
{
  return m_assets_dir / relative_path;
}

std::string
AssetManager::GetAbsolutePathStr(const char* relative_path) const
{
  return (m_assets_dir / relative_path).string();
}

std::string
AssetManager::GetAbsolutePathStr(std::string_view relative_path) const
{
  return (m_assets_dir / relative_path).string();
}
