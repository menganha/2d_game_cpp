#ifndef INC_2D_CPP_ECS_GAME_ENEMYSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_ENEMYSYSTEM_HPP

#include "../Components.hpp"
#include "../Enemy.hpp"

#include <entt/fwd.hpp>
#include <vector>

class EnemySystem
{
    // It takes care (in the moment) of the enemy loading for each level and some enemy behaviour

  public:
    EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher);

    void Update();

  private:
    struct EnemyHorde
    {
        EnemyType type;
        Counter timer;
        Counter amount;
        Counter delay;
    };
    std::vector<EnemyHorde> m_hordes;
    entt::registry&         m_registry;
    entt::dispatcher&       m_dispatcher;
    Counter                 m_timer;
};

#endif // INC_2D_CPP_ECS_GAME_ENEMYSYSTEM_HPP
