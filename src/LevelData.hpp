#pragma once 

#include <string>
#include <vector>
#include "Enemy.hpp"

struct LevelData
  {
    std::string             video;
    std::string             music;
    std::vector<EnemyEntry> enemy_list_to_dispatch;
    std::string             text;
  };

