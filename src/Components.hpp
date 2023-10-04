#ifndef INC_2D_CPP_ECS_GAME_COMPONENTS_HPP
#define INC_2D_CPP_ECS_GAME_COMPONENTS_HPP

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

using namespace entt::literals;

// TODO: Maybe not necessary. We will keep track of the player with its entity ID
using Movable = entt::tag<"Movable"_hs>;

struct Position
{
    float x;
    float y;
};

struct Velocity
{
    float dx;
    float dy;
};


struct Enemy
{
    int type;
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

struct Health
{
    int points;
};

struct Misile
{
    int power;
};

#endif // INC_2D_CPP_ECS_GAME_COMPONENTS_HPP
