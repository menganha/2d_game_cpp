#include "CleanUpSystem.hpp"

#include "../Components.hpp"
#include "../Events.hpp"

CleanUpSystem::CleanUpSystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher} {};

void
CleanUpSystem::Update()
{
  auto to_destroy_view = m_registry.view<Death>();
  for (auto entity : to_destroy_view) {
    m_dispatcher.enqueue(DestroyEvent{entity});
  }
  m_registry.destroy(to_destroy_view.begin(), to_destroy_view.end());
}
