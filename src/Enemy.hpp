#pragma once
#include "Components.hpp"

#include <entt/entt.hpp>

enum class EnemyType
{
  NOTYPE = 0, // Serves just as a flag
  SIMPLE,     // Just moves Down
  SEEKER,
};

struct Enemy
{
  EnemyType type;
};

namespace EnemyUtils {

void
CreateEnemy(entt::registry& registry, EnemyType enemy_type, int initial_pos_x, int initial_pos_y);

void
CreateBullet(entt::registry& registry, const Position& position, const Velocity& velocity, const Collider& collider, int power);

}
