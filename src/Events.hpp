#ifndef INC_2D_CPP_ECS_GAME_EVENTS_HPP
#define INC_2D_CPP_ECS_GAME_EVENTS_HPP

#include "Gamepad.hpp"
#include <SDL2/SDL.h>

struct DirectionalButtonEvent {
    int up;
    int down;
    int left;
    int right;
};

#endif //INC_2D_CPP_ECS_GAME_EVENTS_HPP
