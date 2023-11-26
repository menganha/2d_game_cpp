#pragma once

#include "Font.hpp"

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
    AssetManager() = default;

    // id of font is simply the relative path of the font resource
    void                  AddFont(std::string_view relative_path, int size, SDL_Renderer* renderer);
    Font                  GetFont(std::string_view relative_path) const;
    std::filesystem::path GetAbsolutePath(std::string_view relative_path) const;
    std::string           GetAbsolutePathStr(std::string_view relative_path) const;

private:
    std::filesystem::path                          m_root_dir;
    std::map<entt::hashed_string::hash_type, Font> m_text_cache;
};
