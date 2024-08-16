#include "ScriptSystem.hpp"
#include "../Script.hpp"

#include "../Events.hpp"

ScriptSystem::ScriptSystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
{}

void
ScriptSystem::Update()
{
  auto script_view = m_registry.view<ScriptContainer>();
  for (auto entity: script_view) {
    auto script_container = script_view.get<ScriptContainer>(entity);
    script_container.instance->Update();
    if (script_container.instance->HasFinished()) {
      m_dispatcher.enqueue(ScriptFinishedEvent{script_container.lua_thread});
      m_registry.erase<ScriptContainer>(entity); // erasing it on the loop, but entt supports it
    }
  }
}

bool
ScriptSystem::PendingScripts()
{
  return not m_registry.view<ScriptContainer>().empty();
}

