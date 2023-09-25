#ifndef INC_2D_CPP_ECS_GAME_GAMEPAD_HPP
#define INC_2D_CPP_ECS_GAME_GAMEPAD_HPP

#include <SDL2/SDL.h>
#include <unordered_map>
#include <array>

class Gamepad {
    // Represents the gamepad states after SDL events have been processed. So far this is only related to a keyboard
    // input, but later we can add a subclass (or sibling class), e.g.,  for joystick input
public:

    enum Button {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        A,
        B,
        X,
        Y,
        START,
        SELECT,
        N_BUTTONS // Number of Buttons
    };

    using KeyMap = std::unordered_map<SDL_Scancode, Button>;

    inline static const KeyMap DEFAULT_KEY_MAP{
            {SDL_SCANCODE_UP,     UP},
            {SDL_SCANCODE_DOWN,   DOWN},
            {SDL_SCANCODE_LEFT,   LEFT},
            {SDL_SCANCODE_RIGHT,  RIGHT},
            {SDL_SCANCODE_Z,      A},
            {SDL_SCANCODE_X,      B},
            {SDL_SCANCODE_A,      Y},
            {SDL_SCANCODE_S,      X},
            {SDL_SCANCODE_RETURN, START},
            {SDL_SCANCODE_C,      SELECT},
    };

    using KeyboardState = std::array<Uint8, Button::N_BUTTONS>;

    explicit Gamepad(KeyMap keymap = DEFAULT_KEY_MAP);

    // Refreshes the internal state of the gamepad
    void Update(const Uint8* sdl_keyboard_state);

    bool IsButtonDown(Button button) const;

    bool IsButtonReleased(Button button) const;

    bool IsButtonPressed(Button button) const;

    bool IsButtonEvent(Button button) const;

    bool IsDirectionalButtonEvent() const;

    Uint8 GetKeyState(Button button) const;

private:
    const KeyMap m_key_map;
    std::array<Uint8, Button::N_BUTTONS> m_previous_keyboard_state;
    KeyboardState m_current_keyboard_state;
};


#endif //INC_2D_CPP_ECS_GAME_GAMEPAD_HPP
