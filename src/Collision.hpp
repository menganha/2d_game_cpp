#ifndef INC_2D_CPP_ECS_GAME_COLLISION_HPP
#define INC_2D_CPP_ECS_GAME_COLLISION_HPP

#include "Components.hpp"

namespace Collision {

    // Checks if the rectangular colliders intersects. If they touch borders, i.e., top borders
    // have the same coordinate as the bottom of the other, it is not counted as a collision
    bool HasCollided(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b);

    // Returns whether the entity with collider b is within collider a
    bool IsFullyContained(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b);
}

#endif // INC_2D_CPP_ECS_GAME_COLLISION_HPP
