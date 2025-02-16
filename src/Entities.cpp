#include "Entities.hpp"

#include "Colors.hpp"
#include "engine/Log.hpp"

entt::entity
entities::CreatePlayer(entt::registry& registry)
{
  auto entity = registry.create();
  LINFO("Creating player entity with id %i", static_cast<int>(entity));
  registry.emplace<Position>(entity, 402.f, 500.f);
  registry.emplace<SquarePrimitive>(entity, 10, 10, Colors::RED);
  registry.emplace<Velocity>(entity, 0.f, 0.f);
  registry.emplace<Collider>(entity, 10, 10, 0, 0, true);
  registry.emplace<Health>(entity, PLAYER_INITIAL_HEALTH, 0, PLAYER_INVINCIBILITY_TIME);
  return entity;
}

void
entities::PlayerShoot(entt::entity player_entity, entt::registry& registry)
{
  auto&          position = registry.get<Position>(player_entity);
  const Velocity velocity{0.f, entities::PLAYER_BULLET_VELOCITY};
  entities::CreateBullet(registry, position, velocity, entities::PLAYER_BULLET_SIZE, entities::PLAYER_BULLET_SIZE);
}

entt::entity
entities::CreateBullet(
  entt::registry& registry, const Position& position, const Velocity& velocity, int size, int power)
{
  auto bullet_entity = registry.create();
  registry.emplace<Position>(bullet_entity, position);
  registry.emplace<Velocity>(bullet_entity, velocity);
  registry.emplace<SquarePrimitive>(bullet_entity, Size{size, size}, SDL_Color{0x00, 0x00, 0x00, 0xFF});
  registry.emplace<Collider>(bullet_entity, size, size);
  registry.emplace<Weapon>(bullet_entity, Weapon{power});
  return bullet_entity;
}

entt::entity
entities::CreateEnemy(entt::registry& registry, EnemyBreed enemy_type, int initial_pos_x, int initial_pos_y)
{
  entt::entity entity{entt::null};
  switch (enemy_type) {
    case EnemyBreed::SIMPLE: {
      entity = registry.create();
      registry.emplace<Position>(entity, static_cast<float>(initial_pos_x), static_cast<float>(initial_pos_y));
      registry.emplace<Velocity>(entity, 0.f, 2.f);
      registry.emplace<Collider>(entity, 20, 20);
      registry.emplace<SquarePrimitive>(entity, 20, 20, Colors::BLUE);
      registry.emplace<Health>(entity, 5);
      registry.emplace<Weapon>(entity, 5, false);
      registry.emplace<Enemy>(entity, enemy_type);
      break;
    }
    case EnemyBreed::SEEKER: {
      entity = registry.create();
      registry.emplace<Position>(entity, static_cast<float>(initial_pos_x), static_cast<float>(initial_pos_y));
      registry.emplace<Velocity>(entity, 0.f, 1.f);
      registry.emplace<Collider>(entity, 20, 20);
      registry.emplace<SquarePrimitive>(entity, 20, 20, Colors::BLUE);
      registry.emplace<Health>(entity, 5);
      registry.emplace<Weapon>(entity, 5, false);
      registry.emplace<Enemy>(entity, enemy_type);
      break;
    }
    default:
      LERROR("Enemy Type = %i Case not implemented", static_cast<int>(enemy_type));
  }
  return entity;
}

void
entities::EnemyShoot(entt::registry& registry, const Position& enemy_position, const Velocity& velocity)
{
  auto           bullet_entity =
    entities::CreateBullet(registry, enemy_position, velocity, entities::ENEMY_BULLET_SIZE, entities::ENEMY_BULLET_POWER);
  registry.emplace<Enemy>(bullet_entity, EnemyBreed::NOTYPE);
}

entt::entity
entities::CreateCamera(entt::registry& registry)
{
  auto entity = registry.create();
  LINFO("Creating camepra entity with id %i", static_cast<int>(entity));
  registry.emplace<Position>(entity, 0.0f, 0.0f);
  return entity;
}
