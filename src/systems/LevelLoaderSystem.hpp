#pragma once

#include "../Enemy.hpp"

#include <entt/entt.hpp>
// #include <sol/sol.hpp>
#include <lua.hpp>
#include <string_view>
#include <vector>

//
// Takes input and creates the levels, i.e., enemies, backgrounds, sounds, etc
//
class LevelLoaderSystem
{
public:
  LevelLoaderSystem(entt::registry& registry, entt::dispatcher& dispatcher);
  void LoadLevel(std::string_view level_data_path);
  void Update();

private:
  void CreateEnemy(entt::registry& registry, EnemyType enemy_type, int pos_x, int pos_y);
  struct EnemyEntry
  {
    EnemyType type;
    int       delay;
    int       pos_x;
    int       pos_y;
  };
  entt::registry&         m_registry;
  entt::dispatcher&       m_dispatcher;
  lua_State*              m_lua;
  std::vector<EnemyEntry> m_enemy_list_to_dispatch;
  int m_level_counter; // FIXME: Could this cause an overflow at some point? Levels should not be so large I guess (?)
};
