#include "HUD.hpp"

#include "Colors.hpp"
#include "Components.hpp"

// FIXME: This should not be a system as it keeps referrences to particular entities
HUD::HUD(entt::registry& registry) : m_registry{ registry }, m_ent_health{ entt::null }, m_ent_player{ entt::null } {}

HUD::~HUD()
{
    m_registry.destroy(m_ent_health);
}

void
HUD::Init(int player_health, entt::entity player_entity)
{
    m_ent_health = m_registry.create();
    m_ent_player = player_entity;
    m_registry.emplace<Position>(m_ent_health, 0.0f, 0.0f);
    m_registry.emplace<Text>(
      m_ent_health, "Health: " + std::to_string(player_health), "fonts/PressStart2P.ttf", Colors::BLACK);
}
void
HUD::OnHealthEvent(HealthEvent health_event)
{
    if (health_event.victim_entity == m_ent_player)
    {
        m_registry.emplace_or_replace<Text>(
          m_ent_health, "Health: " + std::to_string(health_event.health), "fonts/PressStart2P.ttf", Colors::BLACK);
    }
}
