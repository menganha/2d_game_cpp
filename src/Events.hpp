#pragma once

#include "Components.hpp"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

struct DestroyEvent
{
    entt::entity entity;
};

struct HealthEvent
{
    entt::entity victim_entity;
    int          health;
};

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

struct SetEntityPositionEvent
{
    entt::entity entity;
    Position     new_position;
};

struct EndLevelEvent
{};
