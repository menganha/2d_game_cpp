#ifndef INC_2D_CPP_ECS_GAME_COMPONENTS_HPP
#define INC_2D_CPP_ECS_GAME_COMPONENTS_HPP

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

using namespace entt::literals;

using Movable = entt::tag<"Movable"_hs>;

struct Position {
    float x;
    float y;
};

struct Renderable {
    float x;
    float y;
};

struct Velocity {
    float dx;
    float dy;
};

struct Color {
    SDL_Color color;
};


#endif //INC_2D_CPP_ECS_GAME_COMPONENTS_HPP
