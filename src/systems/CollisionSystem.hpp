#pragma once

#include <entt/fwd.hpp>
#include <vector>

struct Grid
{
  int origin_x;
  int origin_y;
  int width;
  int height;
  int cell_width;
  int cell_height;
};

class CollisionSystem
{
  // Checks for collision using a grid-based approach
  // Subdivides the space into a grid of squared cells and checks which entities collider
  // boxes intersect within each cell. Afterward, the collision checks between entities can be
  // reduced to only those that are within one cell

public:
  CollisionSystem(entt::registry& registry, entt::dispatcher& dispatcher, Grid grid);

  void Update();

private:
  using Cells = std::vector<std::vector<entt::entity>>;
  entt::registry&   m_registry;
  entt::dispatcher& m_dispatcher;
  Grid              m_grid;
  Cells             m_cells; // cells containing the entities
  int               m_num_cells_x, m_num_cells_y;
  void              clear_grid_cells();
  void              update_grid_cells();
};
