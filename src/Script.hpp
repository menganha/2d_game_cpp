#pragma once

#include "Components.hpp"

#include <entt/fwd.hpp>
#include <lua.hpp>
#include <memory>

class IScript
{
public:
  virtual ~IScript() = default;
  virtual void Update() = 0; // Return signals that the script is finished.
  virtual bool HasFinished() = 0;
};

struct ScriptContainer
{
  std::shared_ptr<IScript> instance;
  lua_State*               lua_thread = nullptr;
};

class MoveEntityScript : public IScript
{
public:
  MoveEntityScript(entt::registry& registry, entt::entity entity, int x, int y, int ticks_it_takes);
  bool HasFinished() { return m_finished; }
  void Update();

private:
  bool      m_finished;
  int       m_ticks_it_takes;
  int       m_ticks_passed;
  Position  m_final_position;
  Position  m_initial_position;
  Position& m_position;
};

