#pragma once

#include "Components.hpp"

namespace enemy {

    Position GetPosition(const Enemy& enemy, const Position& previous_position);
    void     CreateEnemy(entt::registry& registry, EnemyType enemy_type, int pos_x, int pos_y);

}
