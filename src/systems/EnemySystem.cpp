#include "EnemySystem.hpp"

#include "../Colors.hpp"
#include "../Enemy.hpp"
#include "../Events.hpp"

#include <spdlog/spdlog.h>

EnemySystem::EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{ registry }
  , m_dispatcher{ dispatcher }
{}

void
EnemySystem::Update()
{
    // Generate enemies
    for (const auto&& [entity, horde] : m_registry.view<EnemyHorde>().each())
    {
        horde.delay.Tick();
        if (horde.delay.IsDone())
        {
            horde.every.Tick();
            if (horde.every.IsDone())
            {
                horde.amount.Tick();
                if (horde.type == EnemyType::PARAB)
                {
                    const auto entity = m_registry.create();
                    spdlog::info("Creating new enemy entity {}", static_cast<int>(entity));
                    // This logic should be somehow on the Enemy class
                    m_registry.emplace<Position>(entity, -100.0f, 0.0f);  // Put them outside of the screen
                    m_registry.emplace<Collider>(entity, 20, 20);
                    m_registry.emplace<SquarePrimitive>(entity, 20, 20, Colors::BLUE);
                    m_registry.emplace<Health>(entity, 5);
                    m_registry.emplace<Weapon>(entity, 5, false);
                    Position reach{ 300.f, 400.f };
                    m_registry.emplace<Enemy>(entity, EnemyType::PARAB, reach, 1.f);
                }
                horde.every.Reset();
                if (horde.amount.IsDone())
                {
                    m_registry.destroy(entity);
                }
            }
        }
    }

    // Move entities
    auto enemies = m_registry.view<Enemy, Position>();
    for (auto entity : enemies)
    {
        auto&                  enemy = m_registry.get<Enemy>(entity);
        Position               new_position = EnemyUtils::get_position(enemy);
        SetEntityPositionEvent movement_event{ entity, new_position };
        m_dispatcher.enqueue(movement_event);
        enemy.lifetime += enemy.parametric_vel;
    }
}
