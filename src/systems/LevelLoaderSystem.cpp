#include "LevelLoaderSystem.hpp"

#include "../Events.hpp"
#include "../engine/Log.hpp"

#include <cstring>
#include <string_view>

LevelLoaderSystem::LevelLoaderSystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
  , m_lua{}
  , m_enemy_list_to_dispatch{}
  , m_level_counter{0}
{
}

void
LevelLoaderSystem::LoadLevel(std::string_view level_data_path)
{

  m_enemy_list_to_dispatch.clear();
  m_level_counter = 0;
  m_lua = luaL_newstate();

  if (luaL_loadfile(m_lua, "/home/alvaro/games/2d_cpp_ecs_game/scripts/level1.lua") || lua_pcall(m_lua, 0, 0, 0))
    LOGERROR("Cannot run configuration file: %s", lua_tostring(m_lua, -1));
  lua_getglobal(m_lua, "level");     // TODO: Check if it's a table
  lua_pushnil(m_lua);
  while (lua_next(m_lua, -2) != 0) { // TODO: Check if top table
    lua_rawgeti(m_lua, -1, 1);
    const char* enemy_type_str = lua_tostring(m_lua, -1);
    lua_pop(m_lua, 1);

    lua_rawgeti(m_lua, -1, 2);
    int delay = (int)lua_tonumber(m_lua, -1);
    lua_pop(m_lua, 1);

    lua_rawgeti(m_lua, -1, 3);
    int pos_x = (int)lua_tonumber(m_lua, -1);
    lua_pop(m_lua, 1);

    lua_rawgeti(m_lua, -1, 4);
    int pos_y = (int)lua_tonumber(m_lua, -1);
    lua_pop(m_lua, 1);

    LOGINFO("Reading line %s %i %i %i", enemy_type_str, delay, pos_x, pos_y);
    if (std::strcmp(enemy_type_str, "simple") == 0)
      m_enemy_list_to_dispatch.push_back({EnemyType::SIMPLE, delay, pos_x, pos_y});
    else if (std::strcmp(enemy_type_str, "seeker") == 0)
      m_enemy_list_to_dispatch.push_back({EnemyType::SEEKER, delay, pos_x, pos_y});
    else {
      LOGINFO("Enemy type %s not supported setting it to NOTYPE", enemy_type_str);
      m_enemy_list_to_dispatch.push_back({EnemyType::NOTYPE, delay, pos_x, pos_y});
    }
    lua_pop(m_lua, 1);
  }
  // Sort vector
  std::sort(
    m_enemy_list_to_dispatch.begin(),
    m_enemy_list_to_dispatch.end(), //
    [](const auto& a, const auto& b) { return a.delay > b.delay; });
}

void
LevelLoaderSystem::Update()
{

  if (not m_enemy_list_to_dispatch.empty()) {
    auto last = m_enemy_list_to_dispatch.back();
    while (last.delay <= m_level_counter) {
      EnemyUtils::CreateEnemy(m_registry, last.type, last.pos_x, last.pos_y);
      m_enemy_list_to_dispatch.pop_back();
      if (m_enemy_list_to_dispatch.empty())
        break;
      else
        last = m_enemy_list_to_dispatch.back();
    }
  } else // if all enemies have been dispatched, check if there are still
         // enemies alive and if so send the end level signal
  {
    if (not m_registry.view<Enemy>()) {
      m_dispatcher.enqueue(EndLevelEvent());
    }
  }
  m_level_counter++;
}
