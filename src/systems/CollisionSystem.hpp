#ifndef INC_2D_CPP_ECS_GAME_COLLISIONSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_COLLISIONSYSTEM_HPP

#include <entt/entt.hpp>
#include "../Events.hpp"
#include "../Components.hpp"
#include <vector>

struct Grid {
    int origin_x;
    int origin_y;
    int width;
    int height;
    int cell_width;
    int cell_height;
};


class CollisionSystem {
// Checks for collision using a grid-based approach
// Subdivides the space into a grid of squared cells and checks which entities collider boxes intersect within each cell.
// Afterward, the collision checks between entities can be reduces to only those that are within one cell

public:
    CollisionSystem(entt::registry& registry, entt::dispatcher& dispatcher, Grid grid);

    void Update();

    void OnCollisionEvent(CollisionEvent);


private:
    using Cells = std::vector<std::vector<entt::entity>>;
    entt::registry& m_registry;
    entt::dispatcher& m_dispatcher;
    Grid m_grid;
    Cells m_cells;
    int m_num_cells_x, m_num_cells_y;

    static bool HasCollided(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b);

    // Returns whether the entity with collider b is within collider a
    static bool IsFullyContained(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b);
};


#endif //INC_2D_CPP_ECS_GAME_COLLISIONSYSTEM_HPP