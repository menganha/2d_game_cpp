#ifndef INC_2D_CPP_ECS_GAME_EVENTS_HPP
#define INC_2D_CPP_ECS_GAME_EVENTS_HPP

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

#include "Gamepad.hpp"

struct DirectionalButtonEvent {
    int up;
    int down;
    int left;
    int right;
};

struct ShootEvent {
    entt::entity shooter_entity;
};

struct CollisionEvent {
    entt::entity entity_a;
    entt::entity entity_b;
};

struct OutOfBoundariesEvent {
    entt::entity entity;
};

#endif //INC_2D_CPP_ECS_GAME_EVENTS_HPP