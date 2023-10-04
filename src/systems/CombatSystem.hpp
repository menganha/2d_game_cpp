#ifndef INC_2D_CPP_ECS_GAME_COMBATSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_COMBATSYSTEM_HPP

#include "../Events.hpp"

#include <entt/fwd.hpp>

class CombatSystem
{

  public:
    CombatSystem(entt::registry& registry);

    void Update();

    void OnShootButtonEvent(ShootEvent shoot_event);

    void OnCollisionEvent(CollisionEvent collision_event);

    void OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event);

  private:
    entt::registry& m_registry;
};

#endif // INC_2D_CPP_ECS_GAME_COMBATSYSTEM_HPP
