#include "RenderSystem.hpp"

#include "../Components.hpp"

RenderSystem::RenderSystem(entt::registry& registry)
  : m_registry{registry} {};

void
RenderSystem::Update(AssetManager& asset_manager, SDL_Renderer* renderer, entt::entity camera_entity)
{
  // Render SDL2 textures
  m_registry.sort<Position>([](const Position& lhs, const Position& rhs) { return lhs.z < rhs.z; });
  auto view_renderable = m_registry.view<const Position, const Renderable>();
  view_renderable.use<Position>(); // Uses position component order

  // m_scaling += 0.001f;
  // SDL_RenderSetScale(renderer, m_scaling, m_scaling);

  const auto& camera_pos =
    m_registry.get<Position>(camera_entity); // TODO: Maybe have a reference to it and not request it every frame

  for (auto entity : view_renderable) {
    const auto&  pos = view_renderable.get<Position>(entity);
    const auto&  ren = view_renderable.get<Renderable>(entity);
    SDL_Texture* texture = asset_manager.GetTexture(ren.name);
    SDL_Rect     drect{
      static_cast<int>(pos.x - camera_pos.x), static_cast<int>(pos.y - camera_pos.y), ren.size.x, ren.size.y};
    auto src_rect_ptr = SDL_RectEmpty(&ren.subsection) ? nullptr : &ren.subsection;
    if (ren.color_mod.a != 0)
      SDL_SetTextureColorMod(texture, ren.color_mod.r, ren.color_mod.g, ren.color_mod.b);
    SDL_RenderCopy(renderer, texture, src_rect_ptr, &drect);
  }

  // Render text. TODO: We could probably refactor this call into the above render loop!
  auto view_text = m_registry.view<const Position, FullText>();
  for (auto entity : view_text) {
    auto&       fulltext = view_text.get<FullText>(entity);
    const auto& position = view_text.get<Position>(entity);
    const auto& font = asset_manager.GetFont(fulltext.font_id);
    int         pos_x = static_cast<int>(position.x - camera_pos.x);
    int         pos_y = static_cast<int>(position.y - camera_pos.y);
    pos_x -= 100 * (m_scaling - 1);
    pos_y -= 100 * (m_scaling - 1);
    font.DrawText(fulltext.string, pos_x, pos_y, fulltext.color, renderer);
  }

  // Render primitive shapes
  auto view_primitive = m_registry.view<const Position, const SquarePrimitive>();
  for (auto entity : view_primitive) {
    const auto& [pos, ren] = view_primitive.get<Position, SquarePrimitive>(entity);
    auto p_health = m_registry.try_get<Health>(entity);
    auto alpha = ren.color.a;
    if (p_health) {
      if (p_health->invincibility_timer % BLINKING_PERIOD > 0) {
        alpha = 0X10;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      }
    }
    SDL_SetRenderDrawColor(renderer, ren.color.r, ren.color.g, ren.color.b, alpha);
    SDL_Rect rectangle{
      static_cast<int>(pos.x - camera_pos.x), static_cast<int>(pos.y - camera_pos.y), ren.size.x, ren.size.y};
    SDL_RenderFillRect(renderer, &rectangle);
  }
}
