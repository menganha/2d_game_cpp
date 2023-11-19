#pragma once

#include "Components.hpp"

#include <vector>

enum class EnemyType : int
{
    NOTYPE = 0,
    SIMPLE, // Just moves Down
    PARAB,
};

struct Enemy
{
    EnemyType type;
    float     lifetime; // Keeps track of the time alive for deriving movement
    Position  reach;
    float     parametric_vel;
    float     spread;

    Enemy() = default;
    Enemy(EnemyType t_type);
    Enemy(EnemyType t_type, Position t_reach, float t_vel, float t_spread = 0.05f);
};

// struct EnemyHorde
// {
//     // Represent a bunch of enemies
//     EnemyType type;
//     Counter   every; // how often to instantiate the next enemy
//     Counter   amount;
//     Counter   delay;
// };

//
//  Enemy entry for when we want to specify the enemies on a given level
//
struct EnemyEntry
{
    EnemyType type;
    int   delay;
    int   pos_x;
    int   pos_y;
};

using EnemyList = std::vector<EnemyEntry>;

namespace EnemyUtils {
    Position GetPosition(const Enemy& enemy, const Position& previous_position);
    void     CreateEnemy(entt::registry& registry, EnemyType enemy_type, int pos_x, int pos_y);
}
