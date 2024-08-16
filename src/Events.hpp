#pragma once

#include "Components.hpp"

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

#include <lua.hpp>

struct DestroyEvent
{
    entt::entity entity;
};

// Triggered when health is gained or lost
struct DamageEvent
{
    entt::entity entity;
    int          health;
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

struct ScriptFinishedEvent
{
  lua_State* lua_thread;
};
