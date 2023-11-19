#pragma once

#include "../Enemy.hpp"

#include <entt/fwd.hpp>

//
// It takes care (in the moment) of the enemy loading for each level and some enemy behaviour
//
class EnemySystem
{

public:
    EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher);

    void Update();
    void SetEnemyList(EnemyList enemy_list);

private:
    entt::registry&   m_registry;
    entt::dispatcher& m_dispatcher;
    EnemyList         m_enemy_stage_list;
    int               m_enemy_stage_counter;
};
