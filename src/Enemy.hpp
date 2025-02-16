#pragma once

#include <entt/entt.hpp>

enum class EnemyBreed
{
  NOTYPE = 0, // Serves just as a flag
  SIMPLE,     // Just moves Down
  SEEKER,
};

struct Enemy
{
  EnemyBreed breed;
  int        lifetime = 0;
};

