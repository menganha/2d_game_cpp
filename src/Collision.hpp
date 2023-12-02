#pragma once

#include "Components.hpp"

namespace Collision {

    // Checks if the rectangular colliders intersects. If they touch borders, i.e., top borders
    // have the same coordinate as the bottom of the other, it is not counted as a collision
    bool HasCollided(const Position& pos_a, const Collider& coll_a, const Position& pos_b, const Collider& coll_b);

    // Returns whether the entity with collider b is within collider a
    bool IsFullyContained(const Position& pos_a, const Collider& coll_a, const Position& pos_b, const Collider& coll_b);
}
