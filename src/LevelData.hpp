#pragma once

#include "systems/EnemySystem.hpp"

#include <string>
#include <vector>

inline std::string
intToLevelScript(int level)
{
  return std::string("scripts/level") + std::to_string(level) + std::string(".lua");
}

struct LevelData
{
  std::string             video;
  std::string             music;
  std::vector<EnemyEntry> enemy_list_to_dispatch;
  std::string             text;
};

