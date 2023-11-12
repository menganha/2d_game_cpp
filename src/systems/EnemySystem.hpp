#ifndef INC_2D_CPP_ECS_GAME_ENEMYSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_ENEMYSYSTEM_HPP

#include <entt/fwd.hpp>

class EnemySystem
{
    // It takes care (in the moment) of the enemy loading for each level and some enemy behaviour

  public:
    EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher);

    void Update();

  private:
    entt::registry&   m_registry;
    entt::dispatcher& m_dispatcher;
};

#endif // INC_2D_CPP_ECS_GAME_ENEMYSYSTEM_HPP
