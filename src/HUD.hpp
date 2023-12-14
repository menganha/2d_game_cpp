#pragma once

#include "Events.hpp"

#include <entt/entt.hpp>

//
// Collects the elements to be overlayed over the gameplay screen, a.k.a, heads-up Display
//
class HUD
{
public:
    HUD(entt::registry& registry);
    ~HUD();
    void Refresh(int player_health); // We can have more overloaded Refresh functions for each component of the HUD
    void Create(entt::entity player_entity);
    void OnHealthEvent(DamageEvent health_event);

private:
    entt::registry& m_registry;
    entt::entity    m_ent_health;
    entt::entity    m_ent_player; // NOTE: not owned by HUD!
};
