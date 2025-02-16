#pragma once

#include <lua.hpp>
#include <string>

class LuaState
{
public:
  LuaState();
  LuaState(const char* script_path);
  ~LuaState();
  lua_State*  GetState();
  void        Pop(int index = 1);
  void        ReadScript(const char* script_path);
  const char* GetString(int index = -1);
  int         RawGetInt(int index, int table_index = -1);
  const char* RawGetString(int index, int table_index = -1);
  int         Next(int table_index = -2) { return lua_next(m_lua, table_index); }
  void        GetGlobal(const char* name) { lua_getglobal(m_lua, name); }
  void        PushNil() { lua_pushnil(m_lua); }

private:
  lua_State* m_lua;
};
