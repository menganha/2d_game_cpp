#ifndef INC_2D_CPP_ECS_GAME_EVENTS_HPP
#define INC_2D_CPP_ECS_GAME_EVENTS_HPP

#include "Gamepad.hpp"

struct InputEvent {
    const Gamepad* m_gamepad;

//    InputEvent(Gamepad &gamepad) : m_gamepad{gamepad} {}
};

#endif //INC_2D_CPP_ECS_GAME_EVENTS_HPP
