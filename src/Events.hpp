#ifndef INC_2D_CPP_ECS_GAME_EVENTS_HPP
#define INC_2D_CPP_ECS_GAME_EVENTS_HPP

#include "Components.hpp"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

struct ShootEvent
{
    entt::entity shooter_entity;
};

struct CollisionEvent
{
    entt::entity entity_a;
    entt::entity entity_b;
};

struct OutOfBoundariesEvent
{
    Position     bound_pos;
    Collider     bound_coll;
    entt::entity entity;
};

#endif // INC_2D_CPP_ECS_GAME_EVENTS_HPP
