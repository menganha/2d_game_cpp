#include "LuaContext.hpp"

#include <cstring>
#include <engine/Log.hpp>

LuaContext::LuaContext()
  : m_lua{luaL_newstate()}
  , m_level_data{}
{
  luaL_requiref(m_lua, LUA_GNAME, luaopen_base, 1);
  lua_pop(m_lua, 1); // Pops the libs from the stack
  luaL_requiref(m_lua, LUA_STRLIBNAME, luaopen_string, 1);
  lua_pop(m_lua, 1);
  luaL_requiref(m_lua, k_module_name, luaopen_yage_level, 1);
  lua_pop(m_lua, 1);
  lua_pushstring(m_lua, k_level_data_key);
  lua_pushlightuserdata(m_lua, &m_level_data);
  lua_settable(m_lua, LUA_REGISTRYINDEX);
}

LuaContext::~LuaContext() { lua_close(m_lua); }

LevelData
LuaContext::GetLevelData(const char* script_path)
{
  if (luaL_loadfile(m_lua, script_path) || lua_pcall(m_lua, 0, 0, 0)) { // Checks for correctness in the script
    LERROR("Cannot run configuration file: %s", lua_tostring(m_lua, -1));
    throw std::runtime_error(lua_tostring(m_lua, -1));
  }

  // Sort constructed vector
  std::sort(
    m_level_data.enemy_list_to_dispatch.begin(),
    m_level_data.enemy_list_to_dispatch.end(), //
    [](const auto& a, const auto& b) { return a.delay > b.delay; });

  return m_level_data;
}

int
LuaContext::luaopen_yage_level(lua_State* state)
{
  luaL_newlib(state, yage_level_library);
  return 1;
}

int
LuaContext::set_music(lua_State* state)
{
  const char* music_string = lua_tostring(state, 1);
  lua_getfield(state, LUA_REGISTRYINDEX, k_level_data_key);
  LevelData* level_info = static_cast<LevelData*>(lua_touserdata(state, -1));
  if (music_string)
    level_info->music = music_string;
  else
    level_info->music = "";

  return 0;
}

int
LuaContext::set_video(lua_State* state)
{
  const char* music_string = lua_tostring(state, 1);
  lua_getfield(state, LUA_REGISTRYINDEX, k_level_data_key);
  LevelData* level_info = static_cast<LevelData*>(lua_touserdata(state, -1));
  if (music_string)
    level_info->video = music_string;
  else
    level_info->video = "";

  return 0;
}

int
LuaContext::add_enemy(lua_State* state)
{
  lua_getglobal(state, "string");
  lua_getfield(state, -1, "lower");
  lua_rotate(state, 1, 5); // Moves the first argument to the top
  lua_call(state, 1, 1);

  int         delay = (int)lua_tonumber(state, 1);
  int         pos_x = (int)lua_tonumber(state, 2);
  int         pos_y = (int)lua_tonumber(state, 3);
  const char* breed = lua_tostring(state, -1);

  lua_getfield(state, LUA_REGISTRYINDEX, k_level_data_key);
  LevelData* level_info = static_cast<LevelData*>(lua_touserdata(state, -1));
  EnemyBreed breed_enum;
  if (std::strcmp(breed, "simple") == 0) {
    breed_enum = EnemyBreed::SIMPLE;
  } else if (std::strcmp(breed, "seeker") == 0) {
    breed_enum = EnemyBreed::SEEKER;
  } else if (std::strcmp(breed, "simplebullet") == 0) {
    breed_enum = EnemyBreed::SIMPLEBULLET;
  } else {
    breed_enum = EnemyBreed::NOTYPE;
  }
  level_info->enemy_list_to_dispatch.push_back({breed_enum, delay, pos_x, pos_y});
  return 0;
}

int
LuaContext::add_text(lua_State* state)
{
  const char* text = lua_tostring(state, 1);

  lua_getfield(state, LUA_REGISTRYINDEX, k_level_data_key);
  LevelData* level_info = static_cast<LevelData*>(lua_touserdata(state, -1));
  if (text)
    level_info->text = text;
  else
    level_info->text = "";
  return 0;
}

