#include "LuaState.hpp"

#include "engine/Log.hpp"

#include <stdexcept>

LuaState::LuaState()
  : m_lua{luaL_newstate()}
{
}

LuaState::LuaState(const char* script_path)
  : m_lua{luaL_newstate()}
{
  ReadScript(script_path);
}

LuaState::~LuaState() { lua_close(m_lua); }

lua_State*
LuaState::GetState()
{
  return m_lua;
}

void
LuaState::Pop(int index)
{
  lua_pop(m_lua, index);
}

int
LuaState::RawGetInt(int index, int table_index)
{
  lua_rawgeti(m_lua, table_index, index);
  return (int)lua_tonumber(m_lua, -1);
}

const char*
LuaState::RawGetString(int index, int table_index)
{
  lua_rawgeti(m_lua, table_index, index);
  return lua_tostring(m_lua, -1);
}

void
LuaState::ReadScript(const char* script_path)
{
  if (luaL_loadfile(m_lua, script_path) || lua_pcall(m_lua, 0, 0, 0)) { // Checks for correctness in the script
    LERROR("Cannot run configuration file: %s", lua_tostring(m_lua, -1));
    throw std::runtime_error(lua_tostring(m_lua, -1));
  }
}


const char*
LuaState::GetString(int index)
{
  // NOTE: Be aareful, the string will not be available always after it has been remove from the stack
  return lua_tostring(m_lua, index);
}

