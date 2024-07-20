#pragma once

#include "../Components.hpp"
#include "../Events.hpp"

#include <entt/fwd.hpp>

class CombatSystem
{

public:
    CombatSystem(entt::registry& registry, entt::dispatcher& dispatcher);
    void Update();
    void OnCollisionEvent(CollisionEvent collision_event);
    void OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event);

private:
    entt::registry&   m_registry;
    entt::dispatcher& m_dispatcher;
    entt::entity      m_player;

    void HandleDealingDamage(Health& health, entt::entity health_ent, Weapon weapon, entt::entity weapont_ent);
};
