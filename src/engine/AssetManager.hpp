#pragma once

#include "Font.hpp"
#include "Texture.hpp"
#include "Video.hpp"

#include <SDL2/SDL.h>
#include <filesystem>
#include <map>

//
// Handles PnG Textures, Fonts, Sounds and Music
// Id of file-type resources is simply the relative path
//
class AssetManager
{

public:
    AssetManager(const char* assets_dir);

    // Font
    void        AddFont(const char* relative_path, int size, SDL_Renderer* renderer, const char* alias = nullptr);
    const Font& GetFont(std::string_view relative_path) const;

    // Video
    void   AddVideo(const char* relative_path, SDL_Renderer* renderer); // Adds also a texture with the same id
    Video& GetVideo(std::string_view relative_path);

    // Texture
    void     AddTexture(const char* id, SDL_Texture* sdl_texture); // Adds texture by copying the raw SDL texture pointer
    void     AddTexture(const char* id);                           // Adds default Texture
    Texture& GetTexture(std::string_view relative_path);

    std::filesystem::path GetAbsolutePath(const char* relative_path) const;
    std::string           GetAbsolutePathStr(const char* relative_path) const;

private:
    std::filesystem::path                       m_assets_dir;
    std::map<std::string, Font, std::less<>>    m_text_cache;
    std::map<std::string, Video, std::less<>>   m_video_cache;
    std::map<std::string, Texture, std::less<>> m_texture_cache;
};
