#pragma once

#include <entt/entt.hpp>
#include "Events.hpp"

//
// Collects the elements to be overlayed over the gameplay screen, a.k.a, heads-up Display
//
class HUD
{
public:
    HUD(entt::registry& registry);
    ~HUD();
    void Init(int player_health, entt::entity player_entity);
    void OnHealthEvent(HealthEvent health_event);

private:
    entt::registry& m_registry;
    entt::entity m_ent_health;
    entt::entity m_ent_player;
};
