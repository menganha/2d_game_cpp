#include "CollisionSystem.hpp"
#include <spdlog/spdlog.h>
#include <cmath>

CollisionSystem::CollisionSystem(entt::registry& registry, entt::dispatcher& dispatcher, Grid grid) :
        m_registry{registry},
        m_dispatcher{dispatcher},
        m_grid{grid} {
    // We check that it always takes one row/column of cells more if it doesn't fit exactly in the input height and width
    m_num_cells_x = (m_grid.width % m_grid.cell_width) ? m_grid.width / m_grid.cell_width + 1 : m_grid.width / m_grid.cell_width;
    m_num_cells_y = (m_grid.height % m_grid.cell_height) ? m_grid.height / m_grid.cell_height + 1 : m_grid.height / m_grid.cell_height;
    m_cells.resize(m_num_cells_x * m_num_cells_y);
};

void CollisionSystem::Update() {

    Position grid_position{m_grid.origin_x, m_grid.origin_y};
    Collider grid_collider{m_grid.width, m_grid.height, 0, 0};
    auto colliders = m_registry.view<Position, Collider>();
    for (auto entity: colliders) {
        const auto entity_coll = colliders.get<Collider>(entity);
        const auto entity_pos = colliders.get<Position>(entity);
        if (not IsFullyContained(grid_position, grid_collider, entity_pos, entity_coll)) {
            m_dispatcher.enqueue<OutOfBoundariesEvent>(entity);
        }
        int counter = 0;
        for (auto& cell: m_cells) {
            int origin_x = m_grid.origin_x + (counter % m_num_cells_x) * m_grid.cell_width;
            int origin_y = m_grid.origin_y + (counter / m_num_cells_x) * m_grid.cell_height;
            Position cell_pos{origin_x, origin_y};
            Collider cell_coll{m_grid.cell_width, m_grid.cell_height, 0, 0};
            if (HasCollided(cell_pos, cell_coll, entity_pos, entity_coll)) {
                cell.push_back(entity);
            }
            counter += 1;
        }
    }
    for (const auto& cell: m_cells) {
        for (auto it = std::begin(cell); it != std::end(cell); ++it) {
            auto& coll_i = colliders.get<Collider>(*it);
            auto& pos_i = colliders.get<Position>(*it);
            for (auto jt = it; jt != std::end(cell); ++jt) {
                if (*it == *jt) continue;
                auto& coll_j = colliders.get<Collider>(*jt);
                auto& pos_j = colliders.get<Position>(*jt);
                if (HasCollided(pos_i, coll_i, pos_j, coll_j)) {
                    m_dispatcher.enqueue<CollisionEvent>(*it, *jt);
                }
            }
        }
    }

    for (auto& cell: m_cells) {
        cell.clear();
    }

}

bool CollisionSystem::IsFullyContained(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b) {

    int a_min, a_max, b_min, b_max;
    // Horizontal
    a_min = pos_a.x + coll_a.x_offset;
    a_max = a_min + coll_a.width;
    b_min = pos_b.x + coll_b.x_offset;
    b_max = b_min + coll_b.width;
    if (b_min < a_min or b_max > a_max) {
        return false;
    }

    // Vertical
    a_min = pos_a.y + coll_a.y_offset;
    a_max = a_min + coll_a.height;
    b_min = pos_b.y + coll_b.y_offset;
    b_max = b_min + coll_b.height;
    if (b_min < a_min or b_max > a_max) {
        return false;
    }

    return true;
}

bool CollisionSystem::HasCollided(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b) {
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