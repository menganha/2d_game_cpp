#include "Gamepad.hpp"

Gamepad::Gamepad() :
        m_key_map{DEFAULT_KEY_MAP},
        m_previous_keyboard_state{0},
        m_current_keyboard_state{0} {
}

void Gamepad::Update(const Uint8 *sdl_keyboard_state) {
    m_previous_keyboard_state = m_current_keyboard_state;
    for (const auto &[sdl_scan_code, button]: DEFAULT_KEY_MAP) {
        m_current_keyboard_state[button] = sdl_keyboard_state[sdl_scan_code];
    }
}

bool Gamepad::IsButtonDown(Gamepad::Button button) const {
    return m_current_keyboard_state[button];
}

bool Gamepad::IsButtonReleased(Gamepad::Button button) const {
    return not m_current_keyboard_state[button] and m_previous_keyboard_state[button];
}

bool Gamepad::IsButtonPressed(Gamepad::Button button) const {
    return m_current_keyboard_state[button] and m_previous_keyboard_state[button];
}