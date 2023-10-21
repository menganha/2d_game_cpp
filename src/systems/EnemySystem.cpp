#include "EnemySystem.hpp"

#include "../Enemy.hpp"
#include "../Events.hpp"

#include <spdlog/spdlog.h>
#include <type_traits>

const static SDL_Color BLUE{ 0x00, 0x00, 0xFF, 0xFF };

EnemySystem::EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{ registry }
  , m_dispatcher{ dispatcher }
  , m_timer{}
{
    // initializes the hordes
    for (int idx = 1; idx <= 10; ++idx)
    {
        EnemyType type = EnemyType::PARAB;
        // if (idx % 2)
        // {
        //     type = EnemyType::PARAB;
        // }
        // else
        // {
        //     type = EnemyType::BOMBER;
        // }

        EnemyHorde horde{ type, { 35 }, { 7 }, { (idx - 1) * 500 } };
        m_hordes.push_back(horde);
    }
    spdlog::info("Counter is_pod {}", std::is_pod<Counter>::value);
    spdlog::info("Counter is_trivial {}", std::is_trivial<Counter>::value);
    spdlog::info("Counter is_aggregate {}", std::is_aggregate<Counter>::value);
    spdlog::info("Counter is_standard_layout {}", std::is_standard_layout<Counter>::value);
}

void
EnemySystem::Update()
{
    // Generate enemys. TODO: Maybe it can be moved to the level loader
    for (auto& horde : m_hordes)
    {
        horde.delay.Tick();
        if (horde.delay.IsDone())
        {
            horde.timer.Tick();
            if (horde.timer.IsDone())
            {
                if (horde.type == EnemyType::PARAB)
                {
                    const auto entity = m_registry.create();
                    spdlog::info("Creating new enemy entity {}", static_cast<int>(entity));
                    // This logic should be on the Enemy class
                    m_registry.emplace<Position>(entity, 0.0f, 0.0f);
                    m_registry.emplace<Collider>(entity, 20, 20);
                    m_registry.emplace<Renderable>(entity, 20, 20, BLUE);
                    m_registry.emplace<Health>(entity, 5);
                    int amount  = horde.amount.GetCurrentCount();
                    Position reach { 200.f + 40* amount, 400.f };
                    m_registry.emplace<Enemy>(entity, EnemyType::PARAB, reach, 1.f);
                }
                horde.amount.Tick();
                horde.timer.Reset();
            }
        }
    }

    m_hordes.erase(std::remove_if(m_hordes.begin(),
                                  m_hordes.end(),
                                  [](const EnemyHorde _horde) -> bool { return _horde.amount.IsDone(); }),
                   m_hordes.end());

    // Move entities
    auto enemies = m_registry.view<Enemy, Position>();

    for (auto entity : enemies)
    {
        auto&        enemy = m_registry.get<Enemy>(entity);
        Position     new_position = EnemyUtils::get_position(enemy);
        SetEntityPositionEvent movement_event{ entity, new_position };
        m_dispatcher.enqueue(movement_event);
        enemy.lifetime += enemy.parametric_vel;
    }
}
