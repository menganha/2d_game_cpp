#pragma once

#include <SDL2/SDL.h>

//
// Wrapper around and SDL2 texture in order to RAII
//
// TODO: Include a pointer to the renderer on each instance of this class.
class Texture
{
public:
    Texture() = default;
    Texture(SDL_Texture* texture);
    Texture(Texture&& other);
    ~Texture();
    Texture&     operator=(Texture&& other);
    SDL_Texture* GetTexture() { return m_texture; }
    int          GetWidth() { return m_native_width; }
    int          GetHeight() { return m_native_height; }

private:
    SDL_Texture* m_texture;
    int          m_native_width;
    int          m_native_height;
};
