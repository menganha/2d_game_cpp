#include "EnemySystem.hpp"

#include "../Components.hpp"
#include "../Enemy.hpp"
#include "../Entities.hpp"
#include "../Events.hpp"
#include "cmath"

EnemySystem::EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
  , m_level_counter{0}
  , m_level_ended_event_triggered{false}
{}

void
EnemySystem::Update(entt::entity player_entity)
{
  // Handle enemy behaviour
  auto        enemies = m_registry.view<Enemy, Position, Velocity>();
  const auto* player_position = m_registry.try_get<Position>(player_entity);

  // Enemy behaviour
  for (auto entity : enemies) {
    auto& enemy = enemies.get<Enemy>(entity);
    auto& position = enemies.get<Position>(entity);

    switch (enemy.breed) {
      case EnemyBreed::SEEKER:
        position.x += 100.f * std::sin(static_cast<float>(enemy.lifetime) / (3.1416 * 4)) / (3.1416 * 4);
        break;
      case EnemyBreed::SIMPLE:
        // Only create bullets if the player position is known
        if (enemy.lifetime % 60 == 0 and player_position) {
          float    bullet_vel = 2.f;
          int      d_to_player_x = player_position->x - position.x;
          int      d_to_player_y = player_position->y - position.y;
          int      d_to_player = std::sqrt(d_to_player_x * d_to_player_x + d_to_player_y * d_to_player_y);
          Velocity bullet_vel_vec{d_to_player_x * bullet_vel / d_to_player, d_to_player_y * bullet_vel / d_to_player};
          entities::EnemyShoot(m_registry, position, bullet_vel_vec);
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
      entities::CreateEnemy(m_registry, last.breed, last.pos_x, last.pos_y);
      m_enemy_list_to_dispatch.pop_back();
      if (m_enemy_list_to_dispatch.empty())
        break;
      else
        last = m_enemy_list_to_dispatch.back();
    }
  }
  else if (m_registry.view<Enemy>().empty() and not m_level_ended_event_triggered) {
    // When all enemies have been dispatched check if there are still all alive and if so send the end level signal
    m_dispatcher.enqueue(EndLevelEvent());
    m_level_ended_event_triggered = true;
  }
  m_level_counter++;
}

void
EnemySystem::SetEnemyList(const std::vector<EnemyEntry>& enemy_list)
{
  m_enemy_list_to_dispatch = enemy_list;
  m_level_counter = 0;
  m_level_ended_event_triggered = false;
}
