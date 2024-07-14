#pragma once
#include "Components.hpp"

#include <entt/entt.hpp>

enum class EnemyBreed
{
  NOTYPE = 0,   // Serves just as a flag
  SIMPLE,       // Just moves Down
  SIMPLEBULLET, // Linear bullet
  SEEKER,
  SEEKERBULLET,

};

struct EnemyEntry
{
  EnemyBreed breed;
  int        delay;
  int        pos_x;
  int        pos_y;
};

struct Enemy
{
  EnemyBreed breed;
  int        lifetime = 0;
};


namespace EnemyUtils
{
  // void
  // OnUpdate(
  //   entt::registry& regristry, Enemy& enemy, Position& position, Velocity& velocity, const Position& player_position,
  //   uint64_t ticks);

  void CreateEnemy(entt::registry & registry, EnemyBreed enemy_type, int initial_pos_x, int initial_pos_y);

  void CreateSimpleBullet(
    entt::registry & registry, const Position& position, const Velocity& velocity, const Collider& collider, int power);
  void CreateSimpleBullet(
    entt::registry & registry, const Position& position, const Velocity& velocity, int size, int power);
}
