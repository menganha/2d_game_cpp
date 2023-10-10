#include "EnemySystem.hpp"

#include <spdlog/spdlog.h>

const static SDL_Color BLUE{ 0x00, 0x00, 0xFF, 0xFF };

EnemySystem::EnemySystem(entt::registry& registry)
  : m_registry{ registry }
  , m_timer{ 20 }
{
    // initializes the hordes
    for (int idx = 1; idx <= 10; ++idx)
    {
        EnemyHorde horde{ { 0 }, { 20 }, 7, idx * 100 };
        m_hordes.push_back(horde);
    }
}

void
EnemySystem::Update()
{
    bool var = true;
    for (auto horde : m_hordes)
    {
        horde.delay -= 0;
        spdlog::debug("horde delay: {} ", horde.delay);
        if (horde.delay <= 0)
        {
            horde.timer.countdown += 1;
            if (horde.timer.countdown == horde.timer.time)
            {
                const auto entity = m_registry.create();
                if (var)
                {
                    m_registry.emplace<Position>(entity, 0.0f, 0.0f);
                    m_registry.emplace<Velocity>(entity, 2.0f, 2.0f);
                }
                else
                {
                    m_registry.emplace<Position>(entity, 600.0f, 0.0f);
                    m_registry.emplace<Velocity>(entity, -2.0f, -2.0f);
                }
                var = not var;
                m_registry.emplace<Collider>(entity, 20, 20, 0, 0, false);
                m_registry.emplace<Renderable>(entity, 20, 20, BLUE);
                m_registry.emplace<Health>(entity, 5);
                m_registry.emplace<Enemy>(entity, 0);
            }
        }
    }
}
