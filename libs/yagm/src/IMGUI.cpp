#include "IMGUI.hpp"

#include <algorithm>
#include <cstring>

IMGUI::IMGUI()
    : m_draw_buffer{}
    , m_text_pool{}
    , hot_item_id{0}
    , active_item_id{0}
    , counter{0}
    , m_buffer_size{0}
    , m_text_pool_size{0}
    , m_key_confirmed{false} {};

void
IMGUI::UpdateGUIState(const Gamepad& gamepad)
{

    // For now we only have considered vertical widgets.
    if (gamepad.IsButtonPressed(Gamepad::UP))
    {
        hot_item_id = std::max(hot_item_id - 1, 0);
    }
    if (gamepad.IsButtonPressed(Gamepad::DOWN))
    {
        // At the end of the loop the counter will contain the max amount of widgets.
        // If the counter is negative (as when it is initialized, or there are not widgets, then don't do anything)
        hot_item_id = (counter == 0) ? hot_item_id + 1 : std::min(hot_item_id + 1, counter - 1);
    }
    m_key_confirmed = gamepad.IsButtonPressed(Gamepad::A) ? true : false;

    // Counter is reseted here
    counter = 0;
}

bool
IMGUI::Button(const char* text, int x_pos, int y_pos)
{
    // Sets a common counter for the id
    int id = counter++;

    // Add here item to some buffer cache to draw it in this position
    DrawElement& draw_element = m_draw_buffer[m_buffer_size++];
    draw_element.type = IMGUI::TEXT;
    draw_element.x_pos = x_pos;
    draw_element.y_pos = y_pos;
    draw_element.text.text = alloc_text_in_pool(text);
    draw_element.text.color = {0x00, 0x00, 0x00, 0xFF};

    if (hot_item_id == id)
    {
        // DRAW! Add item to cache to drew a highlight
        DrawElement& draw_element = m_draw_buffer[m_buffer_size++];
        draw_element.type = IMGUI::RECT;
        draw_element.x_pos = x_pos;
        draw_element.y_pos = y_pos;
        draw_element.rectangle.height = 30;
        draw_element.rectangle.width = 200;
        draw_element.rectangle.color = {0x00, 0x00, 0xFF, 0x00};
    }

    // Check if is being pressed
    if (m_key_confirmed & (hot_item_id == id))
    {
        active_item_id = id;
        return true;
    }

    // Return False otherwise
    return false;
}

void
IMGUI::Render(const Font& font, SDL_Renderer* renderer)
{
    for (const auto& cmd : m_draw_buffer)
    {
        switch (cmd.type)
        {
            case IMGUI::TEXT:
                font.DrawText(cmd.text.text, cmd.x_pos, cmd.y_pos, cmd.text.color, renderer);
                break;
            case IMGUI::RECT: {
                SDL_SetRenderDrawColor(
                  renderer, cmd.rectangle.color.r, cmd.rectangle.color.g, cmd.rectangle.color.b, SDL_ALPHA_OPAQUE);
                SDL_Rect rectangle{cmd.x_pos, cmd.y_pos, cmd.rectangle.width, cmd.rectangle.height};
                SDL_RenderDrawRect(renderer, &rectangle);
            }
            case IMGUI::INVALID:
                break;
        }
    }

    // Restart
    m_buffer_size = 0;
    m_text_pool_size = 0;
}

const char*
IMGUI::alloc_text_in_pool(const char* text)
{
    auto len = strlen(text) + 1;
    if (m_text_pool_size + len >= kStringPoolSize)
    {
        return nullptr;
    }
    char* dest = &m_text_pool[m_text_pool_size];
    strncpy(dest, text, len);
    m_text_pool_size += len;
    return dest;
}
