#pragma once

#include "Events.hpp"
#include "LevelData.hpp"
#include "entt/fwd.hpp"

#include <lua.hpp>

class LuaContext
{
public:
  LuaContext(); // Loads the API
  ~LuaContext();
  void GetLevelData(const char* script_path, LevelData& level_data, entt::registry&, entt::dispatcher&, entt::entity);

private:
  lua_State* m_lua;

  // Key on registry table where we store the Level info.
  static constexpr const char* k_module_name = "yage";
  static constexpr const char* k_level_data = "yage.level_data";
  static constexpr const char* k_registry = "yage.registry";
  static constexpr const char* k_dispatcher = "yage.dispatcher";
  static constexpr const char* k_player_entity = "yage.player_entity";

  // Expected potential user-defined functions
  static constexpr const char* k_fun_on_level_end = "on_level_end";

  // Callbacks on events
  void UserOnLevelEnd(EndLevelEvent event);
  void OnScriptFinished(ScriptFinishedEvent event);
  void ResumeThread(lua_State* lua_thread);

  // Functions to export
  using FunctionExportedToLua = int(lua_State* state);
  static FunctionExportedToLua     add_enemy, set_music, set_video, add_text, move_player, luaopen_yage_level;
  static constexpr struct luaL_Reg yage_level_library[] = {
    {  "add_enemy",   add_enemy},
    {   "add_text",    add_text},
    {  "set_music",   set_music},
    {  "set_video",   set_video},
    {"move_player", move_player},
    {         NULL,        NULL}
  };
};

