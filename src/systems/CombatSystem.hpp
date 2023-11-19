#ifndef INC_2D_CPP_ECS_GAME_COMBATSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_COMBATSYSTEM_HPP

#include "../Components.hpp"
#include "../Events.hpp"

#include <entt/fwd.hpp>

class CombatSystem
{

public:
    CombatSystem(entt::registry& registry, entt::dispatcher& dispatcher);
    void Update();
    void OnShootEvent(ShootEvent shoot_event);
    void OnCollisionEvent(CollisionEvent collision_event);
    void OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event);

private:
    entt::registry&   m_registry;
    entt::dispatcher& m_dispatcher;
    void HandleDealingDamage(Health& health, entt::entity health_ent, Weapon weapon, entt::entity weapont_ent);
};

#endif // INC_2D_CPP_ECS_GAME_COMBATSYSTEM_HPP
