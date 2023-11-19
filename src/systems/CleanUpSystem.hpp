#pragma once

#include <entt/entt.hpp>
#include "../Events.hpp"


//
//  The only job is to defer the handling of an entity destruction to an independent sytsem which
//  Events are called at a specific time
//
class CleanUpSystem
{
public:
    CleanUpSystem(entt::registry& registry);
    void OnDestroyEvent(DestroyEvent destroy_event);

private:
    entt::registry& m_registry;
};
