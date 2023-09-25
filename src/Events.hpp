#ifndef INC_2D_CPP_ECS_GAME_EVENTS_HPP
#define INC_2D_CPP_ECS_GAME_EVENTS_HPP

#include "Gamepad.hpp"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

struct DirectionalButtonEvent {
    int up;
    int down;
    int left;
    int right;
};

struct CollisionEvent {
    entt::entity entity_a;
    entt::entity entity_b;
};

#endif //INC_2D_CPP_ECS_GAME_EVENTS_HPP
