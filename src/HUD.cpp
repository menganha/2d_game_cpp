#include "HUD.hpp"

#include "Colors.hpp"
#include "Components.hpp"
#include "Config.hpp"

HUD::HUD(entt::registry& registry) : m_registry{registry}, m_ent_health{} {}

HUD::~HUD()
{
    if (m_registry.valid(m_ent_health))
        m_registry.destroy(m_ent_health);
}

void
HUD::Create(entt::entity player_entity)
{
    m_ent_player = player_entity;
    m_ent_health = m_registry.create();
    m_registry.emplace<Position>(m_ent_health, 0.0f, 0.0f);
}

void
HUD::Refresh(int player_health)
{
    m_registry.emplace_or_replace<FullText>(
      m_ent_health, "Health: " + std::to_string(player_health), Config::font_s, Colors::BLUE
      // TODO: BIG Do not specify here the font here!!!
    );
}

void
HUD::OnHealthEvent(DamageEvent health_event)
{
    if (health_event.entity == m_ent_player)
        Refresh(health_event.health);
}
