#ifndef INC_2D_CPP_ECS_GAME_ENEMYSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_ENEMYSYSTEM_HPP

#include "../Components.hpp"

#include <entt/fwd.hpp>
#include <vector>

class EnemySystem
{
    // It takes care (in the moment) of the enemy loading for each level and some enemy behaviour

  public:
    EnemySystem(entt::registry& registry);

    void Update();

  private:
    struct EnemyHorde
    {
        Enemy enemy;
        Timer timer;
        int   amount;
        int delay;
    };
    std::vector<EnemyHorde> m_hordes;
    entt::registry&         m_registry;
    Timer                   m_timer;
};

#endif // INC_2D_CPP_ECS_GAME_ENEMYSYSTEM_HPP
