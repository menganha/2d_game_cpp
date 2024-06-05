#include "CollisionSystem.hpp"

#include "../Collision.hpp"
#include "../Components.hpp"
#include "../Events.hpp"

CollisionSystem::CollisionSystem(entt::registry& registry, entt::dispatcher& dispatcher, Grid grid)
  : m_registry{ registry }
  , m_dispatcher{ dispatcher }
  , m_grid{ grid }
{
  // We check that it always takes one row/column of cells more if it doesn't fit exactly
  // in the input height and width
  m_num_cells_x =
    (m_grid.width % m_grid.cell_width) ? m_grid.width / m_grid.cell_width + 1 : m_grid.width / m_grid.cell_width;
  m_num_cells_y =
    (m_grid.height % m_grid.cell_height) ? m_grid.height / m_grid.cell_height + 1 : m_grid.height / m_grid.cell_height;
  m_cells.resize(m_num_cells_x * m_num_cells_y);
}

//
// Updates the grid and find all collisions between entities
//
void
CollisionSystem::Update()
{
  clear_grid_cells();
  update_grid_cells();

  // Check for collisions
  auto colliders = m_registry.view<Position, Collider>();
  for (const auto& cell : m_cells) {
    for (auto it = std::begin(cell); it != std::end(cell); ++it) {
      auto& coll_i = colliders.get<Collider>(*it);
      auto& pos_i = colliders.get<Position>(*it);
      for (auto jt = it; jt != std::end(cell); ++jt) {
        if (*it == *jt)
          continue;
        auto& coll_j = colliders.get<Collider>(*jt);
        auto& pos_j = colliders.get<Position>(*jt);
        if (Collision::HasCollided(pos_i, coll_i, pos_j, coll_j)) {
          m_dispatcher.enqueue<CollisionEvent>(*it, *jt);
        }
      }
    }
  }
}

void
CollisionSystem::update_grid_cells()
{
  Position grid_position{ static_cast<float>(m_grid.origin_x), static_cast<float>(m_grid.origin_y) };
  Collider grid_collider{
    {m_grid.width, m_grid.height}
  };
  auto colliders = m_registry.view<Position, Collider>();
  for (auto entity : colliders) {
    const auto entity_coll = colliders.get<Collider>(entity);
    const auto entity_pos = colliders.get<Position>(entity);

    // Check if entities are out of the boundary defined by the grid collider
    if (not Collision::IsFullyContained(grid_position, grid_collider, entity_pos, entity_coll)) {
      m_dispatcher.enqueue<OutOfBoundariesEvent>(grid_position, grid_collider, entity);
    }

    // Assigns entities to the grid they belong
    int counter = 0;
    for (auto& cell : m_cells) {
      int      origin_x = m_grid.origin_x + (counter % m_num_cells_x) * m_grid.cell_width;
      int      origin_y = m_grid.origin_y + (counter / m_num_cells_x) * m_grid.cell_height;
      Position cell_pos{ static_cast<float>(origin_x), static_cast<float>(origin_y) };
      Collider cell_coll{
        {m_grid.cell_width, m_grid.cell_height}
      };
      if (Collision::HasCollided(cell_pos, cell_coll, entity_pos, entity_coll)) {
        cell.push_back(entity);
      }
      counter += 1;
    }
  }
}

void
CollisionSystem::clear_grid_cells()
{
  for (auto& cell : m_cells) {
    cell.clear();
  }
}
