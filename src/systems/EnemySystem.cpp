#include "EnemySystem.hpp"

#include "../Enemy.hpp"
#include "../Events.hpp"

#include <spdlog/spdlog.h>

EnemySystem::EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
{}

void
EnemySystem::Update()
{

    // Handle enemy behaviour
    auto enemies = m_registry.view<Enemy, Position>();
    for (auto entity : enemies)
    {
        auto&                  enemy = m_registry.get<Enemy>(entity);
        auto&                  previous_position = m_registry.get<Position>(entity);
        Position               new_position = enemy::GetPosition(enemy, previous_position);
        SetEntityPositionEvent movement_event{entity, new_position};
        m_dispatcher.enqueue(movement_event);
        enemy.lifetime += enemy.parametric_vel;
    }
}
