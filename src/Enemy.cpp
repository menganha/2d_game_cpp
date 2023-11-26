#include "Enemy.hpp"

#include "Colors.hpp"

#include <spdlog/spdlog.h>

Position
enemy::GetPosition(const Enemy& enemy, const Position& previous_position)
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
            float y = previous_position.y + 2.0f;
            return Position{x, y};
        }
    }
}

void
enemy::CreateEnemy(entt::registry& registry, EnemyType enemy_type, int pos_x, int pos_y)
{
    switch (enemy_type)
    {
        case EnemyType::SIMPLE: {
            auto entity = registry.create();
            registry.emplace<Position>(
              entity, static_cast<float>(pos_x), static_cast<float>(pos_y)
            ); // Put them outside of the screen
            registry.emplace<Collider>(entity, 20, 20);
            registry.emplace<SquarePrimitive>(entity, 20, 20, Colors::BLUE);
            registry.emplace<Health>(entity, 5);
            registry.emplace<Weapon>(entity, 5, false);
            registry.emplace<Enemy>(entity, EnemyType::SIMPLE);
            spdlog::info("Creating new enemy entity {} of type SIMPLE", static_cast<int>(entity));
            break;
        }
        default:
            spdlog::error("Enemy Type = {} Case not implemented", static_cast<int>(enemy_type));
    }
}
