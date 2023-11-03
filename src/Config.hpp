#pragma once

#include <SDL2/SDL.h>

namespace Config {
    inline constexpr char   WINDOW_NAME[] = "Unnamed 2d cpp ecs game";
    inline constexpr int    SCREEN_SIZE_X = 800;
    inline constexpr int    SCREEN_SIZE_Y = 600;
    inline constexpr int    COLLISION_GRID_CELL_HEIGHT = 40;
    inline constexpr int    COLLISION_GRID_CELL_WIDTH = 40;
    inline constexpr Uint32 FLAGS =
      SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_ALLOW_HIGHDPI;
}
