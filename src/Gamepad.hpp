#ifndef INC_2D_CPP_ECS_GAME_GAMEPAD_HPP
#define INC_2D_CPP_ECS_GAME_GAMEPAD_HPP

#include <SDL2/SDL.h>
#include <unordered_map>
#include <array>


class Gamepad {
    // Represents the gamepad states after SDL events have been processed
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

    inline static const std::unordered_map<SDL_Scancode, Button> DEFAULT_KEY_MAP{
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

    Gamepad();

    // Refreshes the internal state of the gamepad
    void Update(const Uint8 *sdl_keyboard_state);

    bool IsButtonDown(Button button) const;

    bool IsButtonReleased(Button button) const;

    bool IsButtonPressed(Button button) const;

private:
    const std::unordered_map<SDL_Scancode, Button> m_key_map;
    std::array<Uint8, N_BUTTONS> m_previous_keyboard_state;
    std::array<Uint8, N_BUTTONS> m_current_keyboard_state;
};


#endif //INC_2D_CPP_ECS_GAME_GAMEPAD_HPP
