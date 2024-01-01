#include "AssetManager.hpp"

AssetManager::AssetManager(std::string_view root_dir) : m_root_dir{root_dir} {}

void
AssetManager::AddFont(std::string_view relative_path, int size, SDL_Renderer* renderer)
{
    auto font_path = m_root_dir / "assets" / relative_path;
    m_text_cache.try_emplace(entt::hashed_string::value(relative_path.data()), font_path.c_str(), size, renderer);
}

const Font&
AssetManager::GetFont(std::string_view relative_path) const
{
    return m_text_cache.at(entt::hashed_string::value(relative_path.data()));
}

void
AssetManager::AddVideo(std::string_view relative_path, SDL_Renderer* renderer)
{
    AddTexture(relative_path); // adds a texture with the same ID containing the streaming texture
    auto video_path = m_root_dir / "assets" / relative_path;
    m_video_cache.try_emplace(
      entt::hashed_string::value(relative_path.data()), video_path.c_str(), GetTexture(relative_path), renderer);
}

Video&
AssetManager::GetVideo(std::string_view relative_path)
{
    return m_video_cache.at(entt::hashed_string::value(relative_path.data()));
}

void
AssetManager::AddTexture(std::string_view id, SDL_Texture* sdl_texture)
{
    m_texture_cache.try_emplace(entt::hashed_string::value(id.data()), sdl_texture);
}

void

AssetManager::AddTexture(std::string_view id)
{
    m_texture_cache.try_emplace(entt::hashed_string::value(id.data()));
}

Texture&
AssetManager::GetTexture(std::string_view relative_path)
{
    return m_texture_cache.at(entt::hashed_string::value(relative_path.data()));
}

std::filesystem::path
AssetManager::GetAbsolutePath(std::string_view relative_path) const
{
    return m_root_dir / relative_path;
}

std::string
AssetManager::GetAbsolutePathStr(std::string_view relative_path) const
{
    return (m_root_dir / relative_path).string();
}
