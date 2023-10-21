#include "Enemy.hpp"

Enemy::Enemy(EnemyType t_type, Position t_reach, float t_vel, float t_spread)
  : type{ t_type }
  , reach{ t_reach }
  , parametric_vel{ t_vel }
  , spread{ t_spread }
{
    lifetime = -std::sqrt(reach.y / spread) + reach.x;
}

Position
EnemyUtils::get_position(const Enemy& enemy)
{
    // cases for each enemy type
    float    x = enemy.lifetime;
    float    y = -enemy.spread * std::pow(x - enemy.reach.x, 2) + enemy.reach.y;
    Position position{ x, y };
    return position;
}
