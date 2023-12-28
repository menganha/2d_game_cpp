#pragma once

#include "Font.hpp"
#include "Video.hpp"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <filesystem>
#include <map>
#include <string_view>

//
// Handles PnG Textures, Fonts, Sounds and Music
//
class AssetManager
{

public:
    AssetManager(std::string_view root_dir);

    // Id of file-type resources is simply the relative path
    void AddFont(std::string_view relative_path, int size, SDL_Renderer* renderer);
    Font GetFont(std::string_view relative_path) const;

    void  AddVideo(std::string_view relative_path, SDL_Renderer* renderer);
    Video GetVideo(std::string_view relative_path) const;

    std::filesystem::path GetAbsolutePath(std::string_view relative_path) const;
    std::string           GetAbsolutePathStr(std::string_view relative_path) const;

private:
    std::filesystem::path                           m_root_dir;
    std::map<entt::hashed_string::hash_type, Font>  m_text_cache;
    std::map<entt::hashed_string::hash_type, Video> m_video_cache;
};
