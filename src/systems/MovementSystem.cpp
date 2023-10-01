#include "MovementSystem.hpp"

#include "../Components.hpp"

#include <spdlog/spdlog.h>

MovementSystem::MovementSystem(entt::registry& registry, int screen_width, int screen_height)
  : m_registry{ registry }
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
MovementSystem::OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event)
{
    if (m_registry.all_of<Movable, Position, Velocity>(out_of_boundaries_event.entity)) {
        auto& pos = m_registry.get<Position>(out_of_boundaries_event.entity);
        auto  vel = m_registry.get<Velocity>(out_of_boundaries_event.entity);
        pos.x = pos.x - vel.dx;
        pos.y = pos.y - vel.dy;
        spdlog::info("OutOfBoundariesEvent {} {} {} {}", pos.x, pos.y, vel.dx, vel.dy);
    }
}

void
MovementSystem::OnDirectionalButtonEvent(DirectionalButtonEvent directional_button_event)
{
    auto view = m_registry.view<const Movable, Velocity>();
    for (auto entity : view) {
        auto& velocity = view.get<Velocity>(entity);
        velocity.dx = (directional_button_event.right - directional_button_event.left) * 2;
        velocity.dy = (directional_button_event.down - directional_button_event.up) * 2;
    }
}
