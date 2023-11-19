#include "EnemySystem.hpp"

#include "../Events.hpp"

#include <spdlog/spdlog.h>

EnemySystem::EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{ registry }
  , m_dispatcher{ dispatcher }
  , m_enemy_stage_list{}
  , m_enemy_stage_counter{}
{}

void
EnemySystem::SetEnemyList(EnemyList enemy_stage_list)
{
    m_enemy_stage_list = std::move(enemy_stage_list);
    std::sort(m_enemy_stage_list.begin(), m_enemy_stage_list.end(), [](const auto& a, const auto& b) {
        return a.delay > b.delay;
    });
    m_enemy_stage_counter = 0;
}

void
EnemySystem::Update()
{

    auto enemies = m_registry.view<Enemy, Position>();

    if (not m_enemy_stage_list.empty())
    {
        auto last = m_enemy_stage_list.back();
        while (last.delay <= m_enemy_stage_counter)
        {
            EnemyUtils::CreateEnemy(m_registry, last.type, last.pos_x, last.pos_y);
            m_enemy_stage_list.pop_back();
            if (m_enemy_stage_list.empty())
                break;
            else
                last = m_enemy_stage_list.back();
        }
    }
    else // if all enemies have been dispatched, check if there are still enemies alive and if so send the end level signal
    {
        if (not enemies)
        {
            m_dispatcher.enqueue(EndLevelEvent());
        }
    }

    // Handle enemy behaviour
    for (auto entity : enemies)
    {
        auto&                  enemy = m_registry.get<Enemy>(entity);
        auto&                  previous_position = m_registry.get<Position>(entity);
        Position               new_position = EnemyUtils::GetPosition(enemy, previous_position);
        SetEntityPositionEvent movement_event{ entity, new_position };
        m_dispatcher.enqueue(movement_event);
        enemy.lifetime += enemy.parametric_vel;
    }

    // update the stage counter
    m_enemy_stage_counter++;
}
