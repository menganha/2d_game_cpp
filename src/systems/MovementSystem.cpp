#include "MovementSystem.hpp"

#include "engine/Log.hpp"
#include "../Components.hpp"

#include <entt/entt.hpp>

MovementSystem::MovementSystem(entt::registry& registry, int screen_width, int screen_height)
  : m_registry{registry}
  , m_screen_width{screen_width}
  , m_screen_height{screen_height} {};

void
MovementSystem::Update()
{

  auto accelerables = m_registry.view<Velocity, Acceleration>();
  for (auto entity : accelerables) {
    auto& vel = accelerables.get<Velocity>(entity);
    auto  acc = accelerables.get<Acceleration>(entity);
    vel.x += acc.x;
    vel.y += acc.y;
  }

  auto movables = m_registry.view<Position, const Velocity>();
  for (auto entity : movables) {
    auto&       pos = movables.get<Position>(entity);
    const auto& vel = movables.get<Velocity>(entity);
    pos.x += vel.x;
    pos.y += vel.y;
  }
}

void
MovementSystem::SetEntityPosition(const Position& new_position, entt::entity entity)
{
  auto* position = m_registry.try_get<Position>(entity);
  if (position == nullptr)
    LERROR("Trying to set the position of entity %i without position component", static_cast<int>(entity));
  else {
    position->x = new_position.x;
    position->y = new_position.y;
  }
}

void
MovementSystem::MoveEntity(const Position& displacement, entt::entity entity)
{
  auto* position = m_registry.try_get<Position>(entity);
  if (position == nullptr)
    LERROR("Trying to move entity %i without position component", static_cast<int>(entity));
  else {
    position->x += displacement.x;
    position->y += displacement.y;
  }
}

void
MovementSystem::OnSetEntityPositionEvent(const SetEntityPositionEvent& move_entity_event)
{
  SetEntityPosition(move_entity_event.new_position, move_entity_event.entity);
}

void
MovementSystem::OnOutOfBoundariesEvent(const OutOfBoundariesEvent& out_of_boundaries_event)
{
  // Resolves collision
  // TODO: move this into its own method on the collision system
  LDEBUG("Resolving out of boundaries event with id %i", static_cast<int>(out_of_boundaries_event.entity));
  const auto collider = m_registry.get<Collider>(out_of_boundaries_event.entity);
  if (collider.solid) {
    auto& pos = m_registry.get<Position>(out_of_boundaries_event.entity);
    int   a_min, a_max, b_min, b_max;
    a_min = static_cast<int>(out_of_boundaries_event.bound_pos.x) + out_of_boundaries_event.bound_coll.offset.x;
    a_max = a_min + out_of_boundaries_event.bound_coll.size.x;
    b_min = static_cast<int>(pos.x) + collider.offset.x;
    b_max = b_min + collider.size.x;

    if (b_min < a_min)
      pos.x = a_min - collider.offset.x;
    else if (b_max > a_max)
      pos.x = a_max - collider.offset.x - collider.size.x;

    a_min = static_cast<int>(out_of_boundaries_event.bound_pos.y) + out_of_boundaries_event.bound_coll.offset.y;
    a_max = a_min + out_of_boundaries_event.bound_coll.size.y;
    b_min = static_cast<int>(pos.y) + collider.offset.y;
    b_max = b_min + collider.size.y;
    if (b_min < a_min)
      pos.y = a_min - collider.offset.y;
    else if (b_max > a_max)
      pos.y = a_max - collider.offset.y - collider.size.x;
  }
}
