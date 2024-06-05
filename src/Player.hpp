#pragma once

#include "Components.hpp"

#include "Events.hpp"
#include <entt/entt.hpp>

//
// Player entity wrapper
//
class Player
{
public:
    constexpr static int   PLAYER_INITIAL_HEALTH = 15;
    constexpr static float PLAYER_VELOCITY = 2.0f;
    constexpr static int   PLAYER_INVINCIBILITY_TIME = 120;  // in frames
    Player(entt::registry& registry);
    Position&              GetPosition() const;
    entt::entity           GetEntity() const { return m_entity; }
    void                   Create();
    void                   Shoot();
    void                   OnDamageEvent(DamageEvent health_event);

private:
    entt::registry& m_registry;
    entt::entity    m_entity;
};
