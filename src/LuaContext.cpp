#include "LuaContext.hpp"

#include "Script.hpp"

#include <cstring>
#include <engine/Log.hpp>
#include <memory>

LuaContext::LuaContext()
  : m_lua{luaL_newstate()}
{
  luaL_requiref(m_lua, LUA_GNAME, luaopen_base, 1);
  lua_pop(m_lua, 1);
  luaL_requiref(m_lua, LUA_STRLIBNAME, luaopen_string, 1);
  lua_pop(m_lua, 1);
  luaL_requiref(m_lua, LUA_COLIBNAME, luaopen_coroutine, 1);
  lua_pop(m_lua, 1);
  luaL_requiref(m_lua, k_module_name, luaopen_yage_level, 1);
  lua_pop(m_lua, 1);
}

LuaContext::~LuaContext() { lua_close(m_lua); }

void
LuaContext::GetLevelData(
  const char* script_path, LevelData& level_data, entt::registry& registry, entt::dispatcher& dispatcher,
  entt::entity player_entity)
{
  // Push the pointer to the stack
  lua_pushstring(m_lua, k_level_data);
  lua_pushlightuserdata(m_lua, &level_data);
  lua_settable(m_lua, LUA_REGISTRYINDEX);

  lua_pushstring(m_lua, k_player_entity);
  lua_pushinteger(m_lua, static_cast<int>(player_entity));
  lua_settable(m_lua, LUA_REGISTRYINDEX);

  lua_pushstring(m_lua, k_registry);
  lua_pushlightuserdata(m_lua, &registry);
  lua_settable(m_lua, LUA_REGISTRYINDEX);

  // Clear elements needed
  level_data.music = "";
  level_data.text = "";
  level_data.video = "";
  level_data.enemy_list_to_dispatch.clear();

  if (luaL_dostring(m_lua, "function on_level_end() end"))
    LERROR("Could not set empty on_level_end function: %s", lua_tostring(m_lua, -1));

  // load script
  if (luaL_loadfile(m_lua, script_path) || lua_pcall(m_lua, 0, 0, 0)) {
    LERROR("Cannot run configuration file: %s", lua_tostring(m_lua, -1));
    throw std::runtime_error(lua_tostring(m_lua, -1)); // TODO: Is throwing here necessary?
  }

  // Check if on_level_end is a function
  lua_getglobal(m_lua, k_fun_on_level_end);
  if (lua_isfunction(m_lua, -1)) {
    dispatcher.sink<EndLevelEvent>().connect<&LuaContext::UserOnLevelEnd>(this);
  }
  else { // TODO: This condition does make sense
    LERROR("No user function %s defined on the input script", k_fun_on_level_end);
  }

  dispatcher.sink<ScriptFinishedEvent>().connect<&LuaContext::OnScriptFinished>(this);

  // Sort constructed vector
  std::sort(
    level_data.enemy_list_to_dispatch.begin(),
    level_data.enemy_list_to_dispatch.end(), //
    [](const auto& a, const auto& b) { return a.delay > b.delay; });
}

void
LuaContext::OnScriptFinished(ScriptFinishedEvent script_finished)
{
  ResumeThread(script_finished.lua_thread);
}

void
LuaContext::UserOnLevelEnd([[maybe_unused]] EndLevelEvent end_level_event)
{
  lua_State* lua_thread{lua_newthread(m_lua)};
  lua_getglobal(lua_thread, k_fun_on_level_end);
  ResumeThread(lua_thread);
}

void
LuaContext::ResumeThread(lua_State* lua_thread)
{
  auto status = lua_status(lua_thread);
  if ((status == LUA_OK and lua_gettop(lua_thread) != 0) or status == LUA_YIELD) {
    int  nresults{};
    auto res = lua_resume(lua_thread, m_lua, 0, &nresults);
    if (res == LUA_OK) {
      LINFO("Lua thread %p finalized: %s", (void*)lua_thread, lua_tostring(m_lua, -1));
      lua_closethread(lua_thread, m_lua);
    }
    else if (res == LUA_YIELD) {
      LINFO("Lua thread %p yielded: %s", (void*)lua_thread, lua_tostring(m_lua, -1));
    }
    else {
      LERROR("Lua thread %p error: %s", (void*)lua_thread, lua_tostring(m_lua, -1));
    }
  }
  else
    LDEBUG("Thread %p is on an invalid state %i. It cannot be resumed", (void*)lua_thread, status);
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
  lua_getfield(state, LUA_REGISTRYINDEX, k_level_data);
  LevelData* level_info = static_cast<LevelData*>(lua_touserdata(state, -1));
  if (music_string)
    level_info->music = music_string;
  return 0;
}

int
LuaContext::set_video(lua_State* state)
{
  const char* video_string = lua_tostring(state, 1);
  lua_getfield(state, LUA_REGISTRYINDEX, k_level_data);
  LevelData* level_info = static_cast<LevelData*>(lua_touserdata(state, -1));
  if (video_string)
    level_info->video = video_string;
  return 0;
}

int
LuaContext::move_player(lua_State* state)
{
  int target_x = static_cast<int>(lua_tonumber(state, 1));
  int target_y = static_cast<int>(lua_tonumber(state, 2));
  int time_it_takes = static_cast<int>(lua_tonumber(state, 3));

  lua_getfield(state, LUA_REGISTRYINDEX, k_registry);
  entt::registry* p_registry = (entt::registry*)lua_touserdata(state, -1);

  lua_getfield(state, LUA_REGISTRYINDEX, k_player_entity);
  entt::entity player_entity = (entt::entity)lua_tointeger(state, -1);

  LINFO("Passing state %p to move_player", (void*)state);
  p_registry->emplace<ScriptContainer>(
    player_entity, std::make_shared<MoveEntityScript>(*p_registry, player_entity, target_x, target_y, time_it_takes),
    state);

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

  lua_getfield(state, LUA_REGISTRYINDEX, k_level_data);
  LevelData* level_info = static_cast<LevelData*>(lua_touserdata(state, -1));
  EnemyBreed breed_enum;
  if (std::strcmp(breed, "simple") == 0) {
    breed_enum = EnemyBreed::SIMPLE;
  }
  else if (std::strcmp(breed, "seeker") == 0) {
    breed_enum = EnemyBreed::SEEKER;
  }
  else if (std::strcmp(breed, "simplebullet") == 0) {
    breed_enum = EnemyBreed::SIMPLEBULLET;
  }
  else {
    breed_enum = EnemyBreed::NOTYPE;
  }
  level_info->enemy_list_to_dispatch.push_back({breed_enum, delay, pos_x, pos_y});
  return 0;
}

int
LuaContext::add_text(lua_State* state)
{
  const char* text = lua_tostring(state, 1);

  lua_getfield(state, LUA_REGISTRYINDEX, k_level_data);
  LevelData* level_info = static_cast<LevelData*>(lua_touserdata(state, -1));
  if (text)
    level_info->text = text;
  return 0;
}

