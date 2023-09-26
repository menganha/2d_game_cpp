#include "RenderSystem.hpp"
#include "../Components.hpp"


RenderSystem::RenderSystem(entt::registry& registry) :
        m_registry{registry},
        m_renderer{nullptr} {};

void RenderSystem::Init(SDL_Renderer* renderer) {
    m_renderer = renderer;
}

void RenderSystem::Update() {
    auto view = m_registry.view<const Position, const Renderable>();
    view.each([this](auto& pos, const auto& ren) {
        SDL_SetRenderDrawColor(this->m_renderer, ren.color.r, ren.color.g, ren.color.b, ren.color.a);
        SDL_Rect rectangle{static_cast<int>(pos.x), static_cast<int>(pos.y), ren.width, ren.height};
        SDL_RenderFillRect(this->m_renderer, &rectangle);
    });
    SDL_SetRenderDrawColor(this->m_renderer, 0xCD, 0xDB, 0xDD, 0xE6);
    for (int i_x = 0; i_x < 20; ++i_x) {
        for (int i_y = 0; i_y < 15; ++i_y) {
            SDL_Rect rect{i_x * 40, i_y * 40, 40, 40};
            SDL_RenderDrawRect(this->m_renderer, &rect);
        }
    }
}