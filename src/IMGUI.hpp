#pragma once

#include "SDL2/SDL.h"
#include "engine/Gamepad.hpp"
#include "engine/AssetManager.hpp"

#include <array>

// struct GUIState
// {
//     int hot_item_id;
//     int active_item_id;
// };

class IMGUI
{
public:
    IMGUI(const Gamepad& gamepad);
    void UpdateGUIState();
    bool Button(const char* text, int x_pos, int y_pos);
    void Render(AssetManager& asset_manager, SDL_Renderer* renderer);

private:
    struct ButtonData
    {
        const char* text;
        int         x_pos;
        int         y_pos;
        SDL_Color   color;
        const char* font_id;
    };

    static const int                    kMaxWidgets = 10;
    int                                 hot_item_id; // ids <= -1 are invalid
    int                                 active_item_id;
    int                                 counter;
    Gamepad                             m_gamepad;
    std::array<ButtonData, kMaxWidgets> m_button_widgets;
};
