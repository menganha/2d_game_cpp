#include "MovementSystem.hpp"

#include "../Collision.hpp"
#include "../Components.hpp"

#include <spdlog/spdlog.h>

MovementSystem::MovementSystem(entt::registry& registry,
                               entt::entity    player_entity,
                               int             screen_width,
                               int             screen_height)
  : m_registry{ registry }
  , m_player_entity{ player_entity }
  , m_screen_width{ screen_width }
  , m_screen_height{ screen_height } {};

void
MovementSystem::Update()
{
    auto movables = m_registry.view<Position, Velocity>();

    for (auto entity : movables) {
        auto& pos = m_registry.get<Position>(entity);
        auto& vel = m_registry.get<Velocity>(entity);
        pos.x = pos.x + vel.dx;
        pos.y = pos.y + vel.dy;
        // TODO: This part would have to change to reproduce galaga-style stuff
        if (not m_registry.all_of<Movable>(entity)) { // If not player entity
            if (pos.x > m_screen_width or pos.x < 0) {
                vel.dx = -vel.dx;
            }
            if (pos.y < 0 or pos.y > m_screen_height) {
                vel.dy = -vel.dy;
            }
        }
    }
}

void
MovementSystem::Init(entt::entity player_entity)
{
    m_player_entity = player_entity;
}

void
MovementSystem::OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event)
{
    if (out_of_boundaries_event.entity == m_player_entity) {
        auto& pos = m_registry.get<Position>(out_of_boundaries_event.entity);
        auto  coll = m_registry.get<Collider>(out_of_boundaries_event.entity);
        auto  vel = m_registry.get<Velocity>(out_of_boundaries_event.entity);

        Position tmp_pos{ pos };
        tmp_pos.x = tmp_pos.x - vel.dx;
        if (Collision::IsFullyContained(
              out_of_boundaries_event.bound_pos, out_of_boundaries_event.bound_coll, tmp_pos, coll)) {
            pos = tmp_pos;
            return;
        }
        tmp_pos.x = tmp_pos.x + vel.dx; // return it to its previous value
        tmp_pos.y = tmp_pos.y - vel.dy;
        if (Collision::IsFullyContained(
              out_of_boundaries_event.bound_pos, out_of_boundaries_event.bound_coll, tmp_pos, coll)) {
            pos = tmp_pos;
            return;
        }
        tmp_pos.x = tmp_pos.x - vel.dx; // substract the two values
        if (Collision::IsFullyContained(
              out_of_boundaries_event.bound_pos, out_of_boundaries_event.bound_coll, tmp_pos, coll)) {
            pos = tmp_pos;
            return;
        }

        spdlog::error(
          "OutOfBoundariesEvent was not resolved for player entity {} {} {} {}", pos.x, pos.y, vel.dx, vel.dy);
    }
}

void
MovementSystem::OnDirectionalButtonEvent(DirectionalButtonEvent directional_button_event)
{
    // TODO: This 2 should be a velocity constant set somewhere
    auto& velocity = m_registry.get<Velocity>(m_player_entity);
    velocity.dx = (directional_button_event.right - directional_button_event.left) * 2;
    velocity.dy = (directional_button_event.down - directional_button_event.up) * 2;
}
