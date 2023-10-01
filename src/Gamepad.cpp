#include "Gamepad.hpp"

Gamepad::Gamepad(KeyMap key_map)
  : m_key_map{ std::move(key_map) }
  , m_previous_keyboard_state{}
  , m_current_keyboard_state{}
{
}

void
Gamepad::Update(const Uint8* sdl_keyboard_state)
{
    m_previous_keyboard_state = m_current_keyboard_state;
    for (const auto& [sdl_scan_code, button] : m_key_map) {
        m_current_keyboard_state[button] = sdl_keyboard_state[sdl_scan_code];
    }
}

bool
Gamepad::IsButtonDown(Button button) const
{
    return m_current_keyboard_state[button];
}

bool
Gamepad::IsButtonReleased(Button button) const
{
    return not m_current_keyboard_state[button] and m_previous_keyboard_state[button];
}

bool
Gamepad::IsButtonPressed(Button button) const
{
    return m_current_keyboard_state[button] and not m_previous_keyboard_state[button];
}

bool
Gamepad::IsButtonEvent(Button button) const
{
    return IsButtonReleased(button) or IsButtonPressed(button);
}

bool
Gamepad::IsDirectionalButtonEvent() const
{
    return IsButtonEvent(Button::UP) or IsButtonEvent(Button::DOWN) or IsButtonEvent(Button::LEFT) or
           IsButtonEvent(Button::RIGHT);
}

Uint8
Gamepad::GetKeyState(Button button) const
{
    return m_current_keyboard_state.at(button);
}
