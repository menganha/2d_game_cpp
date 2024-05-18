#include "IMGUI.hpp"

#include <algorithm>

IMGUI::IMGUI(const Gamepad& gamepad) : hot_item_id{-1}, active_item_id{-1}, counter{-1}, m_gamepad{gamepad} {};

void
IMGUI::UpdateGUIState()
{

    // For now we only have considered vertical widgets.
    if (m_gamepad.IsButtonPressed(Gamepad::DOWN))
    {
        hot_item_id = std::max(hot_item_id - 1, 0);
    }
    else if (m_gamepad.IsButtonPressed(Gamepad::UP))
    {
        // At the end of the loop the counter will contain the max amount of widgets.
        // If the counter is negative (as when it is initialized, or there are not widgets, then don't do anything)
        hot_item_id = (counter == -1) ? hot_item_id + 1 : std::min(hot_item_id + 1, counter);
    }

    // Counter is reseted here
    counter = -1;
}

bool
IMGUI::Button(const char* text, int x_pos, int y_pos)
{
    // Sets a common counter for the id
    int id = counter + 1;

    // Add here item to some buffer cache to draw it in this position
    m_button_widgets[id] = ButtonData{text, x_pos, y_pos, {0x00, 0x00, 0x00, 0xFF}, nullptr};

    if (hot_item_id == id)
    {
        // DRAW! Add item to cache to drew a highlight
    }

    // Check if is being pressed
    if (m_gamepad.IsButtonPressed(Gamepad::A) & (hot_item_id == id))
    {
        active_item_id = id;
        return true;
    }

    // Return False otherwise
    return false;
}

void
IMGUI::Render(AssetManager& asset_manager, SDL_Renderer* renderer)
{
    for (const auto& button_w : m_button_widgets)
    {
        const auto& font = asset_manager.GetFont(button_w.font_id);
        font.DrawText(button_w.text, button_w.x_pos, button_w.y_pos, button_w.color, renderer);
    }
}
