#pragma once
#include "../Components.hpp"
#include "../Events.hpp"

#include <entt/fwd.hpp>

class MovementSystem
{

public:
  MovementSystem(entt::registry& registry, int screen_width, int screen_height);

  void Update();

  void MoveEntity(const Position& displacement, entt::entity entity);

  void SetEntityPosition(const Position& new_position, entt::entity entity);

  void OnSetEntityPositionEvent(const SetEntityPositionEvent& move_entity_event);

  void OnOutOfBoundariesEvent(const OutOfBoundariesEvent& out_of_boundaries_event);

private:
  entt::registry& m_registry;
  int             m_screen_width;
  int             m_screen_height;
};

