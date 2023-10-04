#include "Collision.hpp"

#include "Components.hpp"

bool
Collision::IsFullyContained(const Position& pos_a,
                            const Collider& coll_a,
                            const Position& pos_b,
                            const Collider& coll_b)
{
    int a_min, a_max, b_min, b_max;

    // Horizontal Check
    a_min = static_cast<int>(pos_a.x) + coll_a.x_offset;
    a_max = a_min + coll_a.width;
    b_min = static_cast<int>(pos_b.x) + coll_b.x_offset;
    b_max = b_min + coll_b.width;
    if (b_min < a_min or b_max > a_max) {
        return false;
    }

    // Vertical Check
    a_min = static_cast<int>(pos_a.y) + coll_a.y_offset;
    a_max = a_min + coll_a.height;
    b_min = static_cast<int>(pos_b.y) + coll_b.y_offset;
    b_max = b_min + coll_b.height;
    if (b_min < a_min or b_max > a_max) {
        return false;
    }

    return true;
}

bool
Collision::HasCollided(const Position& pos_a, const Collider& coll_a, const Position& pos_b, const Collider& coll_b)
{
    // AABB collision check. Copied from SDL
    int a_min, a_max, b_min, b_max;

    // Horizontal intersection
    a_min = static_cast<int>(pos_a.x) + coll_a.x_offset;
    a_max = a_min + coll_a.width;
    b_min = static_cast<int>(pos_b.x) + coll_b.x_offset;
    b_max = b_min + coll_b.width;

    if (b_min > a_min)
        a_min = b_min;
    if (b_max < a_max)
        a_max = b_max;
    if (a_max <= a_min)
        return false;

    // Vertical intersection
    a_min = static_cast<int>(pos_a.y) + coll_a.y_offset;
    a_max = a_min + coll_a.height;
    b_min = static_cast<int>(pos_b.y) + coll_b.y_offset;
    b_max = b_min + coll_b.height;
    if (b_min > a_min)
        a_min = b_min;
    if (b_max < a_max)
        a_max = b_max;
    if (a_max <= a_min)
        return false;

    return true;
}
