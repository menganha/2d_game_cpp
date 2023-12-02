#pragma once

#include "Components.hpp"

#include <entt/entt.hpp>

//
// Player entity wrapper
//
class Player
{
public:
    Player(entt::registry& registry);
    constexpr static int   kPlayerInitialHealth = 40;
    constexpr static float kPlayerVelocity = 2.0f;
    Position&              GetPosition() const;
    entt::entity           GetEntity() const { return m_entity; }
    void                   Create();
    void                   Shoot();

private:
    entt::registry& m_registry;
    entt::entity    m_entity;
};
