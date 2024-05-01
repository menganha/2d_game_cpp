#pragma once

#include "Font.hpp"
#include "Texture.hpp"
#include "Video.hpp"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <filesystem>
#include <map>
#include <string_view>

//
// Handles PnG Textures, Fonts, Sounds and Music
// Id of file-type resources is simply the relative path
//
class AssetManager
{

public:
    AssetManager(std::filesystem::path root_dir);

    // Font
    void        AddFont(std::string_view relative_path, int size, SDL_Renderer* renderer);
    const Font& GetFont(std::string_view relative_path) const;

    // Video
    void   AddVideo(std::string_view relative_path, SDL_Renderer* renderer); // Adds also a texture with the same id
    Video& GetVideo(std::string_view relative_path);

    // Texture
    void AddTexture(std::string_view id, SDL_Texture* sdl_texture); // Adds texture by copying the raw SDL texture pointer
    void AddTexture(std::string_view id);                           // Adds default Texture
    Texture& GetTexture(std::string_view relative_path);

    std::filesystem::path GetAbsolutePath(std::string_view relative_path) const;
    std::string           GetAbsolutePathStr(std::string_view relative_path) const;

private:
    std::filesystem::path                             m_root_dir;
    std::map<entt::hashed_string::hash_type, Font>    m_text_cache;
    std::map<entt::hashed_string::hash_type, Video>   m_video_cache;
    std::map<entt::hashed_string::hash_type, Texture> m_texture_cache;
};
