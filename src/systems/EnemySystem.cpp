#include "EnemySystem.hpp"

#include "../Components.hpp"
#include "../Enemy.hpp"
#include "../Events.hpp"
#include "cmath"

EnemySystem::EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
  , m_level_counter{0}
{
}

void
EnemySystem::Update(entt::entity player_entity)
{
  // Handle enemy behaviour
  auto        enemies = m_registry.view<Enemy, Position, Velocity>();
  const auto& player_position = m_registry.get<Position>(player_entity);

  // Enemy behaviour

  for (auto entity : enemies) {
    auto& enemy = enemies.get<Enemy>(entity);
    auto& position = enemies.get<Position>(entity);
    // auto& velocity = enemies.get<Velocity>(entity);
    // EnemyUtils::OnUpdate(enemy, position, velocity, player_position);

    int d_to_player_x = player_position.x - position.x;
    int d_to_player_y = player_position.y - position.y;
    int d_to_player = std::sqrt(d_to_player_x * d_to_player_x + d_to_player_y * d_to_player_y);

    switch (enemy.breed) {
      case EnemyBreed::SEEKER:
        position.x += 100.f * std::sin(static_cast<float>(enemy.lifetime) / (3.1416 * 4)) / (3.1416 * 4);
        break;
      case EnemyBreed::SIMPLE:
        if (enemy.lifetime % 60 == 0) {
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
    enemy.lifetime += 1;
  }

  // Creation of enemy entities
  if (not m_enemy_list_to_dispatch.empty()) {
    auto last = m_enemy_list_to_dispatch.back();
    while (last.delay <= m_level_counter) {
      EnemyUtils::CreateEnemy(m_registry, last.breed, last.pos_x, last.pos_y);
      m_enemy_list_to_dispatch.pop_back();
      if (m_enemy_list_to_dispatch.empty())
        break;
      else
        last = m_enemy_list_to_dispatch.back();
    }
  } else if (not m_registry.view<Enemy>()) {
    // If all enemies have been dispatched, check if there are still enemies alive and if so send the end level signal
    m_dispatcher.enqueue(EndLevelEvent());
  }
  m_level_counter++;
}

void
EnemySystem::SetEnemyList(const std::vector<EnemyEntry>& enemy_list)
{
  m_enemy_list_to_dispatch = enemy_list;
  m_level_counter = 0;
}
