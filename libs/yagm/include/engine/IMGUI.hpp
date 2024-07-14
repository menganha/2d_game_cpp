#pragma once

#include "Font.hpp"
#include "Gamepad.hpp"

#include <SDL2/SDL.h>

static const int kMaxDrawElements = 32;
static const int kStringPoolSize = 256;

class IMGUI
{
public:
  IMGUI();
  void UpdateGUIState(const Gamepad& gamepad);
  bool Button(const char* text, int x_pos, int y_pos);
  void Render(const Font& font, SDL_Renderer* renderer);

private:
  enum DrawType
  {
    INVALID = 0,
    TEXT,
    RECT,
  };

  struct Text
  {
    const char* text;
    SDL_Color   color;
  };

  struct Rectangle
  {
    int       width, height;
    SDL_Color color;
  };

  struct DrawElement
  {
    DrawType type;
    int      x_pos;
    int      y_pos;
    union
    {
      Text      text;
      Rectangle rectangle;
    };
  };

  const char* alloc_text_in_pool(const char* text);

  DrawElement m_draw_buffer[kMaxDrawElements];
  char        m_text_pool[kStringPoolSize];
  int         hot_item_id;
  int         active_item_id;
  int         counter;
  std::size_t m_buffer_size;
  std::size_t m_text_pool_size;
  bool        m_key_confirmed;
};
