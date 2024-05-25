#include "Font.hpp"

#include <stdexcept>

Font::Font(const std::string_view font_filepath, int point_size, SDL_Renderer* renderer)
    : m_point_size{point_size}
    , m_glyphs{}
    , m_texture{nullptr}
{
    // SDL_Color white_color{ 0xFF, 0xFF, 0xFF, 0xFF };
    TTF_Font* font{TTF_OpenFont(font_filepath.data(), point_size)};
    if (font == nullptr)
    {
        std::string err_msg = "SDL TTF Error: ";
        throw std::runtime_error(err_msg + TTF_GetError());
    }
    // TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
    // TTF_SetFontStyle(font, TTF_STYLE_ITALIC);

    // Creates a transparent surface with the color masks set as little endian
    SDL_Surface* surface{
      SDL_CreateRGBSurface(0, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF)};

    SDL_Rect destination{0, 0, 0, 0};
    for (char ascii_char = FIRST_ASCII_CHAR; ascii_char <= LAST_ASCII_CHAR; ++ascii_char)
    {
        char char_to_store[2]{ascii_char, 0};

        // This will render the text without antialiasing. Good if we want to have that 8-bit look on fonts. Otherwise 
        //  we should use TTF_RenderUTF8_Blended instead
        SDL_Surface* text_surface{TTF_RenderUTF8_Solid(font, char_to_store, {0xFF, 0xFF, 0xFF, 0xFF})};

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
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

Font::~Font()
{
    SDL_DestroyTexture(m_texture);
    // TTF_CloseFont(m_font);
}

void
Font::DrawText(const std::string_view text, int x, int y, SDL_Color color, SDL_Renderer* renderer) const
{

    SDL_SetTextureColorMod(m_texture, color.r, color.g, color.b);

    for (const char& character : text)
    {
        auto glyph = m_glyphs[character];

        SDL_Rect dest{x, y, glyph.w, glyph.h};

        SDL_RenderCopy(renderer, m_texture, &glyph, &dest);

        x += glyph.w;
    }
}

TextSize Font::GetSize(const std::string_view text){

    TextSize size{0,0};
    for (const char& character : text)
    {
        auto glyph = m_glyphs[character];
        size.w += glyph.w;
        size.h += glyph.h;
    }
    return size;
}

