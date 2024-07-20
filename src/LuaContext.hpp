#pragma once

#include "LevelData.hpp"

#include <lua.hpp>

class LuaContext
{
public:
  LuaContext(); // Loads the API
  ~LuaContext();
  LevelData GetLevelData(const char* script_path);

private:
  lua_State* m_lua;
  LevelData  m_level_data;

  // Key on registry table where we store the Level info.
  static constexpr const char* k_module_name = "yage";
  static constexpr const char* k_level_data_key = "yage.level_data";

  // Functions to export
  using FunctionExportedToLua = int(lua_State* state);
  static FunctionExportedToLua     add_enemy, set_music, set_video, add_text, luaopen_yage_level;
  static constexpr struct luaL_Reg yage_level_library[] = {
    {"add_enemy", add_enemy},
    { "add_text",  add_text},
    {"set_music", set_music},
    {"set_video", set_video},
    {       NULL,      NULL}
  };
};

