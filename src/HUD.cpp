#include "HUD.hpp"

#include "Colors.hpp"
#include "Components.hpp"

HUD::HUD(entt::registry& registry) : m_registry{registry}, m_ent_health{} {}

HUD::~HUD()
{
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
    m_registry.emplace_or_replace<Text>(
      m_ent_health, "Health: " + std::to_string(player_health), "fonts/PressStart2P.ttf", Colors::BLACK
    );
}

void
HUD::OnHealthEvent(DamageEvent health_event)
{
    if (health_event.entity == m_ent_player)
        Refresh(health_event.health);
}
