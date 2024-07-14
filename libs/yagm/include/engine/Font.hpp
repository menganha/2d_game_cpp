#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <string_view>

struct TextSize
{
  int w, h;
};

class Font
{
public:
  Font(const std::string_view font_filepath, int point_size, SDL_Renderer* renderer);
  ~Font();
  void     DrawText(const std::string_view text, int x, int y, SDL_Color color, SDL_Renderer* renderer) const;
  TextSize GetSize(const std::string_view text);

private:
  static const int  N_ASCII_CODES = 128;
  static const char FIRST_ASCII_CHAR = ' ';
  static const char LAST_ASCII_CHAR = '~';
  static const int  FONT_ATLAS_SIZE = 500;

  int                                 m_point_size;
  std::array<SDL_Rect, N_ASCII_CODES> m_glyphs;
  SDL_Texture*                        m_texture;
};
