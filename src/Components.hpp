#ifndef INC_2D_CPP_ECS_GAME_COMPONENTS_HPP
#define INC_2D_CPP_ECS_GAME_COMPONENTS_HPP

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

using namespace entt::literals;

using Movable = entt::tag<"Movable"_hs>; // TODO: Maybe not necessary. We will keep track of the player with its entity ID

struct Position
{
    int x;
    int y;
};

struct Renderable
{
    int       height;
    int       width;
    SDL_Color color;
};

struct Collider
{
    int width;
    int height;
    int x_offset;
    int y_offset;
};

struct Velocity
{
    int dx;
    int dy;
};

struct Health
{
    int points;
};

struct Misile
{
    int power;
};

#endif // INC_2D_CPP_ECS_GAME_COMPONENTS_HPP
