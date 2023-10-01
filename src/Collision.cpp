#include "Collision.hpp"

bool
Collision::IsFullyContained(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b)
{
    int a_min, a_max, b_min, b_max;

    // Horizontal Check
    a_min = pos_a.x + coll_a.x_offset;
    a_max = a_min + coll_a.width;
    b_min = pos_b.x + coll_b.x_offset;
    b_max = b_min + coll_b.width;
    if (b_min < a_min or b_max > a_max) {
        return false;
    }

    // Vertical Check
    a_min = pos_a.y + coll_a.y_offset;
    a_max = a_min + coll_a.height;
    b_min = pos_b.y + coll_b.y_offset;
    b_max = b_min + coll_b.height;
    if (b_min < a_min or b_max > a_max) {
        return false;
    }

    return true;
}

bool
Collision::HasCollided(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b)
{
    // AABB collision check. Copied from SDL
    int a_min, a_max, b_min, b_max;

    // Horizontal intersection
    a_min = pos_a.x + coll_a.x_offset;
    a_max = a_min + coll_a.width;
    b_min = pos_b.x + coll_b.x_offset;
    b_max = b_min + coll_b.width;

    if (b_min > a_min)
        a_min = b_min;
    if (b_max < a_max)
        a_max = b_max;
    if (a_max <= a_min)
        return false;

    // Vertical intersection
    a_min = pos_a.y + coll_a.y_offset;
    a_max = a_min + coll_a.height;
    b_min = pos_b.y + coll_b.y_offset;
    b_max = b_min + coll_b.height;
    if (b_min > a_min)
        a_min = b_min;
    if (b_max < a_max)
        a_max = b_max;
    if (a_max <= a_min)
        return false;

    return true;
}
