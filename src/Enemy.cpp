#include "Enemy.hpp"

#include "Colors.hpp"
#include "Components.hpp"

#include <spdlog/spdlog.h>

void
EnemyUtils::CreateEnemy(entt::registry& registry, EnemyType enemy_type, int initial_pos_x, int initial_pos_y)
{
  // TODO: Put entities initialy outside of the screen by default
  switch (enemy_type) {
    case EnemyType::SIMPLE: {
      auto entity = registry.create();
      registry.emplace<Position>(entity, static_cast<float>(initial_pos_x), static_cast<float>(initial_pos_y));
      registry.emplace<Velocity>(entity, 0.f, 2.f);
      registry.emplace<Collider>(entity, 20, 20);
      registry.emplace<SquarePrimitive>(entity, 20, 20, Colors::BLUE);
      registry.emplace<Health>(entity, 5);
      registry.emplace<Weapon>(entity, 5, false);
      registry.emplace<Enemy>(entity, enemy_type);
      break;
    }
    case EnemyType::SEEKER: {
      auto entity = registry.create();
      registry.emplace<Position>(entity, static_cast<float>(initial_pos_x), static_cast<float>(initial_pos_y));
      registry.emplace<Velocity>(entity, 0.f, 1.f);
      registry.emplace<Acceleration>(entity, 0.0f, 0.0f);
      registry.emplace<Collider>(entity, 20, 20);
      registry.emplace<SquarePrimitive>(entity, 20, 20, Colors::BLUE);
      registry.emplace<Health>(entity, 5);
      registry.emplace<Weapon>(entity, 5, false);
      registry.emplace<Enemy>(entity, enemy_type);
      break;
    }
    default:
      spdlog::error("Enemy Type = {} Case not implemented", static_cast<int>(enemy_type));
  }
}

void
EnemyUtils::CreateBullet(
  entt::registry& registry, const Position& position, const Velocity& velocity, const Collider& collider, int power)
{
  auto bullet_entity = registry.create();

  registry.emplace<Position>(bullet_entity, position);
  registry.emplace<Velocity>(bullet_entity, velocity);
  registry.emplace<SquarePrimitive>(bullet_entity, collider.size, SDL_Color{0x00, 0x00, 0x00, 0xFF});
  registry.emplace<Collider>(bullet_entity, collider);
  registry.emplace<Weapon>(bullet_entity, Weapon{power});
}
