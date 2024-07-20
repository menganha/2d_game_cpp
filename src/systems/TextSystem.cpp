#include "TextSystem.hpp"

#include "../Components.hpp"
#include "../Enemy.hpp"

TextSystem::TextSystem(entt::registry& registry, AssetManager& asset_manager)
  : m_registry{registry}
  , m_asset_manager{asset_manager}
{
}

void
TextSystem::Update()
{
  auto view_text = m_registry.view<Position, TypedText>();
  for (auto entity : view_text) {
    auto& position = view_text.get<Position>(entity);
    auto& t_text = view_text.get<TypedText>(entity);

    if (t_text.delay_counter == 0) {
      const auto& font = m_asset_manager.GetFont(t_text.full_text.font_id);
      char        character = t_text.full_text.string[t_text.index++];
      SDL_Rect    glyph = font.GetGlyph(character);

      if (character == ' ') {
        t_text.pos_x += glyph.w;
      }
      else if (character == '\n') {
        t_text.pos_y += glyph.h;
        t_text.pos_x = 0;
      }
      else {
        auto  renderable_ent = m_registry.create();
        float position_x = position.x + t_text.pos_x;
        float position_y = position.y + t_text.pos_y;
        m_registry.emplace<Position>(renderable_ent, position_x, position_y);
        t_text.pos_x += glyph.w; // move the relative position for the next char

        // The texture ID is the same as the font ID on the asset manager side
        m_registry.emplace<Renderable>(
          renderable_ent, t_text.full_text.font_id, glyph.w, glyph.h, glyph, t_text.full_text.color);

        // Creates enemies
        m_registry.emplace<Weapon>(renderable_ent, 5, false);
        m_registry.emplace<Enemy>(renderable_ent, EnemyBreed::NOTYPE);
        m_registry.emplace<Health>(renderable_ent, 5);
        m_registry.emplace<Collider>(renderable_ent, glyph.w, glyph.h);
      }
      if (t_text.index == static_cast<int>(t_text.full_text.string.size())) {
        m_registry.emplace<Death>(entity);
      }
      t_text.delay_counter = t_text.type_delay - 1;
    }
    else {
      t_text.delay_counter--;
    }
  }
}
