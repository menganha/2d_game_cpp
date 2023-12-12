#pragma once

#include "../Components.hpp"

#include <entt/fwd.hpp>

//
// It takes care (in the moment) of the enemy loading for each level and some enemy behaviour
//
class EnemySystem
{

public:
    EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher);
    void Update(entt::entity player_entity);

private:
    entt::registry&   m_registry;
    entt::dispatcher& m_dispatcher;
    int               m_enemy_stage_counter;
    Velocity          GetBulletVel(float abs_value_vel, Position pos_player, Position pos_initial);
    Position          GetPosition(const Enemy& enemy, const Position& previous_position);
};
