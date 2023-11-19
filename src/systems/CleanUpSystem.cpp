#include "CleanUpSystem.hpp"

CleanUpSystem::CleanUpSystem(entt::registry& registry) : m_registry{ registry } {};

void
CleanUpSystem::OnDestroyEvent(DestroyEvent destroy_event)
{
    m_registry.destroy(destroy_event.entity);
}
