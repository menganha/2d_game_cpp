#include "AssetManager.hpp"

#include <cassert>
#include <filesystem>

AssetManager::AssetManager(const char* assets_dir) : m_assets_dir{std::filesystem::path(assets_dir).parent_path()} {}

void
AssetManager::AddFont(const char* relative_path, int size, SDL_Renderer* renderer, const char* alias)
{
    auto font_path = m_assets_dir / "assets" / relative_path;

    if (alias)
        m_text_cache.try_emplace(alias, font_path.c_str(), size, renderer);
    else
        m_text_cache.try_emplace(relative_path, font_path.c_str(), size, renderer);
}

const Font&
AssetManager::GetFont(std::string_view relative_path) const
{
    auto it = m_text_cache.find(relative_path);
    assert(it != m_text_cache.end());
    return it->second;
}

void
AssetManager::AddVideo(const char* relative_path, SDL_Renderer* renderer)
{
    AddTexture(relative_path); // adds a texture with the same ID containing the streaming texture
    auto video_path = m_assets_dir / "assets" / relative_path;
    m_video_cache.try_emplace(relative_path, video_path.c_str(), GetTexture(relative_path), renderer);
}

Video&
AssetManager::GetVideo(std::string_view relative_path)
{
    auto it = m_video_cache.find(relative_path);
    assert(it != m_video_cache.end());
    return it->second;
}

void
AssetManager::AddTexture(const char* id, SDL_Texture* sdl_texture)
{
    m_texture_cache.try_emplace(id, sdl_texture);
}

void

AssetManager::AddTexture(const char* id)
{
    m_texture_cache.try_emplace(id);
}

Texture&
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
