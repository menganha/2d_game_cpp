#include "Enemy.hpp"

#include "Colors.hpp"
#include "Components.hpp"
#include "engine/Log.hpp"


void
EnemyUtils::CreateEnemy(entt::registry& registry, EnemyBreed enemy_type, int initial_pos_x, int initial_pos_y)
{
  switch (enemy_type) {
    case EnemyBreed::SIMPLE: {
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
    case EnemyBreed::SEEKER: {
      auto entity = registry.create();
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
}

// void
// OnUpdate(
//   entt::registry& registry, Enemy& enemy, Position& position, Velocity& velocity, const Position& player_position,
//   uint64_t ticks)
// {

//   Velocity vector_to_player{player_position.x - position.x, player_position.y - position.y};
//   switch (enemy.type) {
//     case EnemyBreed::SEEKER: {
//       auto& acc = registry.get<Acceleration>(entity);
//       // Cap velocity
//       float velocity_abs = VecNorm(velocity);
//       if (velocity_abs > 2.0f) {
//         velocity.x = velocity.x * (2.0 / velocity_abs);
//         velocity.y = velocity.y * (2.0 / velocity_abs);
//       }
//       // Get desired acceleration
//       auto desired_velocity_x = vector_to_player.x * (2.0f / velocity_abs);
//       auto desired_velocity_y = vector_to_player.y * (2.0f / velocity_abs);
//       acc.x = desired_velocity_x - velocity.x;
//       acc.y = desired_velocity_y - velocity.y;
//       // cap accelereation
//       float abs_acc = VecNorm(acc);
//       if (abs_acc > 1.0f) {
//         acc.x = acc.x * (1.0f / abs_acc);
//         acc.y = acc.y * (1.0f / abs_acc);
//       }
//       break;
//     }
//     default:
//       break;
//   }
// }

void
EnemyUtils::CreateSimpleBullet(
  entt::registry& registry, const Position& position, const Velocity& velocity, const Collider& collider, int power)
{
  auto bullet_entity = registry.create();

  registry.emplace<Enemy>(bullet_entity, EnemyBreed::SIMPLEBULLET);
  registry.emplace<Position>(bullet_entity, position);
  registry.emplace<Velocity>(bullet_entity, velocity);
  registry.emplace<SquarePrimitive>(bullet_entity, collider.size, SDL_Color{0x00, 0x00, 0x00, 0xFF});
  registry.emplace<Collider>(bullet_entity, collider);
  registry.emplace<Weapon>(bullet_entity, Weapon{power});
}

void
EnemyUtils::CreateSimpleBullet(
  entt::registry& registry, const Position& position, const Velocity& velocity, int size, int power)
{
  auto bullet_entity = registry.create();

  registry.emplace<Enemy>(bullet_entity, EnemyBreed::SIMPLEBULLET);
  registry.emplace<Position>(bullet_entity, position);
  registry.emplace<Velocity>(bullet_entity, velocity);
  registry.emplace<SquarePrimitive>(bullet_entity, Size{size, size}, SDL_Color{0x00, 0x00, 0x00, 0xFF});
  registry.emplace<Collider>(bullet_entity, size, size);
  registry.emplace<Weapon>(bullet_entity, Weapon{power});
}

