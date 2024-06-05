#include "EnemySystem.hpp"

#include "cmath"

#include <algorithm>
#include <cstdlib>
#include <spdlog/spdlog.h>

EnemySystem::EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
{
}

void
EnemySystem::Update(entt::entity player_entity)
{

  // Handle enemy behaviour
  auto        enemies = m_registry.view<Enemy, Position, Velocity>();
  const auto& player_position = m_registry.get<Position>(player_entity);

  for (auto entity : enemies) {
    const auto& enemy = enemies.get<Enemy>(entity);
    const auto& position = enemies.get<Position>(entity);
    Velocity    vector_to_player = GetDirectionToPlayer(player_position, position);

    switch (enemy.type) {
      case EnemyType::SEEKER: {
        auto& acc = m_registry.get<Acceleration>(entity);
        auto& velocity = enemies.get<Velocity>(entity);

        // Cap velocity
        float velocity_abs = VecNorm(velocity);
        if (velocity_abs > 2.0f) {
          velocity.x = velocity.x * (2.0 / velocity_abs);
          velocity.y = velocity.y * (2.0 / velocity_abs);
        }
        // Get desired acceleration
        auto desired_velocity_x = vector_to_player.x * (2.0f / velocity_abs);
        auto desired_velocity_y = vector_to_player.y * (2.0f / velocity_abs);
        acc.x = desired_velocity_x - velocity.x;
        acc.y = desired_velocity_y - velocity.y;

        // cap accelereation
        float abs_acc = VecNorm(acc);
        if (abs_acc > 1.0f) {
          acc.x = acc.x * (1.0f / abs_acc);
          acc.y = acc.y * (1.0f / abs_acc);
        }
        break;
      }
      // default:
      //   spdlog::error("Enemy Type = {} Case not implemented", static_cast<int>(enemy.type));
    }

    // Attack
    // Switch statement for every enemy
    if (rand() % 100 < 2) {
      float bullet_velocity = 2.f;
      int   bullet_size = 3;
      int   bullet_power = 5;
      auto  abs_val_vec = VecNorm(vector_to_player);
      vector_to_player.x *= bullet_velocity / abs_val_vec;
      vector_to_player.y *= bullet_velocity / abs_val_vec;
      EnemyUtils::CreateBullet(
        m_registry, position, vector_to_player,
        Collider{
          {bullet_size, bullet_size}
      },
        bullet_power);
    }
  }
}

Velocity
EnemySystem::GetDirectionToPlayer(Position pos_player, Position pos_initial)
{
  float vel_x = pos_player.x - pos_initial.x;
  float vel_y = pos_player.y - pos_initial.y;
  return Velocity{vel_x, vel_y};
}

