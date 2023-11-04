#ifndef INC_2D_CPP_ECS_GAME_COMBATSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_COMBATSYSTEM_HPP

#include "../Events.hpp"

#include <entt/fwd.hpp>
#include <tuple>

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
    template<typename Ta, typename Tb>
    std::tuple<Ta*, Tb*> try_signature(entt::entity entity_a, entt::entity entity_b);

};

#endif // INC_2D_CPP_ECS_GAME_COMBATSYSTEM_HPP
