#include "EnemySystem.hpp"

#include "../Colors.hpp"
#include "../Events.hpp"
#include "cmath"

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
        Position               new_position = GetPosition(enemy, previous_position);
        SetEntityPositionEvent movement_event{entity, new_position};
        m_dispatcher.enqueue(movement_event);
        enemy.lifetime += enemy.parametric_vel;

        // Attack
        // Switch statement for every enemy
        if (rand() % 100 < 2)
        {
            auto     player_position = m_registry.get<Position>(player_entity);
            float    bullet_velocity = 2.f;
            int      bullet_size = 3;
            Velocity velocity = GetBulletVel(bullet_velocity, player_position, new_position);
            // clang-format off
            m_dispatcher.enqueue(BulletEvent{new_position, velocity, Colors::RED, {bullet_size, bullet_size}});
            // clang-format on
        }
    }
}

Velocity
EnemySystem::GetBulletVel(float abs_value_vel, Position pos_player, Position pos_initial)
{
    float vel_x = pos_player.x - pos_initial.x;
    float vel_y = pos_player.y - pos_initial.y;
    float abs_value = std::sqrt(vel_x * vel_x + vel_y * vel_y);
    return Velocity{abs_value_vel * vel_x / abs_value, abs_value_vel * vel_y / abs_value};
}

Position
EnemySystem::GetPosition(const Enemy& enemy, const Position& previous_position)
{
    switch (enemy.type)
    {
        case EnemyType::PARAB: {
            float x = enemy.lifetime;
            float y = -enemy.spread * std::pow(x - enemy.reach.x, 2) + enemy.reach.y;
            return Position{x, y};
        }
        case EnemyType::SIMPLE:
        default: {
            float x = previous_position.x;
            float y = previous_position.y + 2.0f; // TODO: Constant here????
            return Position{x, y};
        }
    }
}
