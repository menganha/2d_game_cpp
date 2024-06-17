#include "EnemySystem.hpp"

#include "../Components.hpp"
#include "../Enemy.hpp"
#include "cmath"

#include <spdlog/spdlog.h>

EnemySystem::EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
{
}

void
EnemySystem::Update(entt::entity player_entity, uint64_t ticks)
{

  // Handle enemy behaviour
  auto        enemies = m_registry.view<Enemy, Position, Velocity>();
  const auto& player_position = m_registry.get<Position>(player_entity);

  for (auto entity : enemies) {
    auto& enemy = enemies.get<Enemy>(entity);
    auto& position = enemies.get<Position>(entity);
    // auto& velocity = enemies.get<Velocity>(entity);
    // EnemyUtils::OnUpdate(enemy, position, velocity, player_position);

    int d_to_player_x = player_position.x - position.x;
    int d_to_player_y = player_position.y - position.y;
    int d_to_player = std::sqrt(d_to_player_x * d_to_player_x + d_to_player_y * d_to_player_y);

    switch (enemy.type) {
      case EnemyType::SEEKER:
        position.x += 200.f * std::sin(static_cast<float>(ticks) / (3.1416 * 4)) / (3.1416 * 4);
        // TODO: Think about starting always from the same phase!!!!
        break;
      case EnemyType::SIMPLE:
        if (ticks % 60 == 0) {
          float    bullet_vel = 2.f;
          int      bullet_size = 3;
          int      bullet_power = 5;
          Velocity bullet_vel_vec{d_to_player_x * bullet_vel / d_to_player, d_to_player_y * bullet_vel / d_to_player};
          EnemyUtils::CreateSimpleBullet(m_registry, position, bullet_vel_vec, bullet_size, bullet_power);
          break;
        }
      default:
        break;
    }
  }
}

