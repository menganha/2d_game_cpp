#pragma once

#include "Components.hpp"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

struct DestroyEvent
{
    entt::entity entity;
};

// Triggered when health is gained or lost
struct HealthEvent
{
    entt::entity entity;
    int          health;
};

struct BulletEvent
{
    Position  position;
    Velocity  velocity;
    SDL_Color color;
    Size      collider_size;
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

struct SetEntityPositionEvent
{
    entt::entity entity;
    Position     new_position;
};

struct EndLevelEvent
{};
