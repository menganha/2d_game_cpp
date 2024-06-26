#include "RenderSystem.hpp"

#include "../Components.hpp"

RenderSystem::RenderSystem(const entt::registry& registry)
  : m_registry{registry} {};

void
RenderSystem::Update(AssetManager& asset_manager, SDL_Renderer* renderer)
{
  // TODO: Improve the renderable data type to include a rendering order (what goes on top of what)

  // Render SDL2 textures
  auto view_renderable = m_registry.view<const Position, const Renderable>();
  for (auto entity : view_renderable) {
    const auto& pos = m_registry.get<Position>(entity); // Add this position somewhere
    const auto& ren = m_registry.get<Renderable>(entity);
    auto&       texture = asset_manager.GetTexture(ren.name);
    SDL_Rect    rect{static_cast<int>(pos.x), static_cast<int>(pos.y), ren.size.x, ren.size.y};
    SDL_RenderCopy(renderer, texture.GetTexture(), nullptr, &rect);
  }

  // Render text
  auto view_text = m_registry.view<const Position, const Text>();
  for (auto entity : view_text) {
    const auto& [position, text] = m_registry.get<Position, Text>(entity);
    const auto& font = asset_manager.GetFont(text.font_id);
    font.DrawText(text.text, static_cast<int>(position.x), static_cast<int>(position.y), text.color, renderer);
  }

  // Render primitive shapes
  auto view_primitive = m_registry.view<const Position, const SquarePrimitive>();
  for (auto entity : view_primitive) {
    const auto& [pos, ren] = m_registry.get<Position, SquarePrimitive>(entity);
    // = m_registry.get<SquarePrimitive>(entity);
    auto p_health = m_registry.try_get<Health>(entity);
    auto alpha = ren.color.a;
    if (p_health) {
      if (p_health->invincibility_timer % BLINKING_PERIOD > 0) {
        alpha = 0X10;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      }
    }
    SDL_SetRenderDrawColor(renderer, ren.color.r, ren.color.g, ren.color.b, alpha);
    SDL_Rect rectangle{static_cast<int>(pos.x), static_cast<int>(pos.y), ren.size.x, ren.size.y};
    SDL_RenderFillRect(renderer, &rectangle);
  }
}
