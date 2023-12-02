#include "EnemySystem.hpp"

#include "../Colors.hpp"
#include "../Enemy.hpp"
#include "../Events.hpp"

#include <cstdlib>
#include <spdlog/spdlog.h>

EnemySystem::EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher)
    : m_registry{registry}
    , m_dispatcher{dispatcher}
{}

void
EnemySystem::Update(entt::entity player_entity)
{

    // Handle enemy behaviour
    auto enemies = m_registry.view<Enemy, Position>();
    for (auto entity : enemies)
    {
        // Move
        auto&                  enemy = m_registry.get<Enemy>(entity);
        auto&                  previous_position = m_registry.get<Position>(entity);
        Position               new_position = enemy::GetPosition(enemy, previous_position);
        SetEntityPositionEvent movement_event{entity, new_position};
        m_dispatcher.enqueue(movement_event);
        enemy.lifetime += enemy.parametric_vel;

        // Attack
        // Switch statement for every enemy
        if (rand() % 100 < 2)
        {
            auto     player_position = m_registry.get<Position>(player_entity);
            auto difference = UnitDifference(player_position, new_position);
            Velocity velocity{difference.x*2, difference.y*2};
            m_dispatcher.enqueue(BulletEvent{
              new_position, velocity, Colors::RED, {2, 2}
            });
        }
    }
}
