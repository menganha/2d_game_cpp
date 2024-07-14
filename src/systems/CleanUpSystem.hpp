#pragma once

#include <entt/entt.hpp>
// #include "../Events.hpp"


//
//  The only job is to defer the handling of an entity destruction to an independent system which
//  does it at a specific time
//
class CleanUpSystem
{
public:
    CleanUpSystem(entt::registry& registry, entt::dispatcher& dispatcher);
    void Update();

private:
    entt::registry& m_registry;
    entt::dispatcher& m_dispatcher;
};
