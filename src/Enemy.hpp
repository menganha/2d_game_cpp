#ifndef INC_2D_CPP_ECS_GAME_ENEMY_HPP
#define INC_2D_CPP_ECS_GAME_ENEMY_HPP

#include "Components.hpp"

enum class EnemyType : int
{
    NOTYPE = 0,
    PARAB,
    CIRC,
    BOMBER
};

struct Enemy
{
    EnemyType type;
    Position  reach;
    float     parametric_vel;
    float     lifetime;
    float     spread;

    Enemy() = default;
    Enemy(EnemyType t_type, Position t_reach, float t_vel, float t_spread = 0.05f);
};

struct EnemyHorde
{
    // Represent a bunch of enemies
    EnemyType type;
    Counter   every; // how often to instantiate the next enemy
    Counter   amount;
    Counter   delay;
};

namespace EnemyUtils {

    Position get_position(const Enemy& enemy);

}

#endif // INC_2D_CPP_ECS_GAME_ENEMY_HPP
