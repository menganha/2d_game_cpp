#include "Texture.hpp"

Texture::Texture(SDL_Texture* texture) : m_texture{texture}, m_native_width{}, m_native_height{}
{
    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_native_width, &m_native_height);
}

Texture::~Texture()
{
    SDL_DestroyTexture(m_texture);
}

Texture::Texture(Texture&& other) : m_texture{nullptr}, m_native_width{}, m_native_height{}
{
    *this = std::move(other);
}

Texture&
Texture::operator=(Texture&& other)
{
    if (this != &other)
    {
        SDL_DestroyTexture(m_texture);

        m_texture = other.m_texture;
        m_native_width = other.m_native_width;
        m_native_height = other.m_native_height;

        other.m_texture = nullptr;
        other.m_native_width = 0;
        other.m_native_height = 0;
    }
    return *this;
}
