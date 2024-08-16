#pragma once

#include <entt/fwd.hpp>
#include <lua.hpp>

class ScriptSystem
{
public:
  ScriptSystem(entt::registry&, entt::dispatcher&);
  void Update();
  bool PendingScripts();

private:
  entt::registry&   m_registry;
  entt::dispatcher& m_dispatcher;
};
