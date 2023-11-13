#include "RenderSystem.hpp"

#include "../Components.hpp"

RenderSystem::RenderSystem(const entt::registry& registry) : m_registry{ registry } {};

void
RenderSystem::Update(const AssetManager& asset_manager, SDL_Renderer* renderer)
{

    // Render text
    auto view_text = m_registry.view<const Position, const Text>();
    view_text.each([renderer, asset_manager](const auto& position, const auto& text) {
        const auto& font = asset_manager.GetFont(text.font_id); // reference or copy????
        font.DrawText(text.text, static_cast<int>(position.x), static_cast<int>(position.y), text.color, renderer);
    });

    // Render primitive shapes
    auto view_primitive = m_registry.view<const Position, const SquarePrimitive>();
    view_primitive.each([renderer](auto& pos, const auto& ren) {
        SDL_SetRenderDrawColor(renderer, ren.color.r, ren.color.g, ren.color.b, ren.color.a);
        SDL_Rect rectangle{ static_cast<int>(pos.x), static_cast<int>(pos.y), ren.width, ren.height };
        SDL_RenderFillRect(renderer, &rectangle);
    });

    // #ifndef NDEBUG
    //     for (int i_x = 0; i_x < 20; ++i_x) {
    //         for (int i_y = 0; i_y < 15; ++i_y) {
    //             SDL_Rect rect{ i_x * 40, i_y * 40, 40, 40 };
    //             SDL_RenderDrawRect(this->m_renderer, &rect);
    //         }
    //     }
    // #endif
}
