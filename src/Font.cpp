#include "Font.hpp"

#include <stdexcept>

Font::Font(const std::string_view font_filepath, int point_size, SDL_Renderer* renderer)
  : m_point_size{ point_size }
  , m_glyphs{}
  , m_texture{ nullptr }
{
    // SDL_Color white_color{ 0xFF, 0xFF, 0xFF, 0xFF };
    TTF_Font* font{ TTF_OpenFont(font_filepath.data(), point_size) };
    if (font == nullptr)
    {
        std::string err_msg = "SDL TTF Error: ";
        throw std::runtime_error(err_msg + TTF_GetError());
    }

    SDL_Surface* surface{ SDL_CreateRGBSurface(0, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, 32, 0, 0, 0, 0xFF) };
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    SDL_Rect destination{ 0, 0, 0, 0 };
    for (char ascii_char = FIRST_ASCII_CHAR; ascii_char <= LAST_ASCII_CHAR; ++ascii_char)
    {
        char char_to_store[2]{ ascii_char, 0 };

        SDL_Surface* text_surface{ TTF_RenderText_Solid(font, char_to_store, { 0xFF, 0xFF, 0xFF, 0xFF }) };

        TTF_SizeText(font, char_to_store, &destination.w, &destination.h);

        if (destination.x + destination.w >= FONT_ATLAS_SIZE)
        {
            destination.x = 0;

            destination.y += destination.h + 1;

            if (destination.y + destination.h >= FONT_ATLAS_SIZE)
            {
                throw std::runtime_error("Out of glyph space in the font atlas texture map");
            }
        }
        m_glyphs[ascii_char] = destination;

        SDL_BlitSurface(text_surface, NULL, surface, &destination);

        SDL_FreeSurface(text_surface);

        destination.x += destination.w;
    }
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    TTF_CloseFont(font);
}

Font::~Font()
{
    // TTF_CloseFont(m_font);
}

void
Font::DrawText(const std::string_view text, int x, int y, SDL_Color color, SDL_Renderer* renderer)
{

    SDL_SetTextureColorMod(m_texture, color.r, color.g, color.b);

    for (const char& character : text)
    {
        auto glyph = m_glyphs[character];

        SDL_Rect dest{ x, y, glyph.w, glyph.h };

        SDL_RenderCopy(renderer, m_texture, &glyph, &dest);

        x += glyph.w;
    }
}

// SDL_Texture*
// Font::RenderText(const std::string& text, SDL_Color color, SDL_Renderer* renderer)
// {
//     SDL_Surface* surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
//     SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//     // TODO: Have some error handling here!!!
//     SDL_FreeSurface(surface);
//     return texture;
// }
