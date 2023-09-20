#include "Gamepad.hpp"
#include <algorithm>

Gamepad::Gamepad() :
        m_key_map{DEFAULT_KEY_MAP},
        m_previous_keyboard_state{0},
        m_current_keyboard_state{0} {
}

void Gamepad::Update(const Uint8 *sdl_keyboard_state) {
    m_previous_keyboard_state = m_current_keyboard_state;
    for (const auto &[sdl_scan_code, button]: m_key_map) {
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

bool Gamepad::IsInputEvent() const {
    return std::any_of(m_key_map.begin(), m_key_map.end(),
                       [this](const auto &pair) {
                           return IsButtonReleased(pair.second) or IsButtonDown(pair.second) or
                                  IsButtonPressed(pair.second);
                       }
    );
}