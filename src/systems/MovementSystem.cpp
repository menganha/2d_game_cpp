#include "MovementSystem.hpp"

#include "../Collision.hpp"

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

MovementSystem::MovementSystem(entt::registry& registry, int screen_width, int screen_height)
  : m_registry{ registry }
  , m_screen_width{ screen_width }
  , m_screen_height{ screen_height } {};

void
MovementSystem::Update()
{
    auto movables = m_registry.view<Position, Velocity>();

    for (auto entity : movables)
    {
        auto& pos = m_registry.get<Position>(entity);
        auto  vel = m_registry.get<Velocity>(entity);
        pos.x = pos.x + vel.dx;
        pos.y = pos.y + vel.dy;
    }
}

void
MovementSystem::SetEntityPosition(const Position& new_position, entt::entity entity)
{
    auto* position = m_registry.try_get<Position>(entity);
    if (position == nullptr)
        spdlog::error("Trying to set the position of entity {} without position component", static_cast<int>(entity));
    else
    {
        position->x = new_position.x;
        position->y = new_position.y;
    }
}

void
MovementSystem::MoveEntity(const Position& displacement, entt::entity entity)
{
    auto* position = m_registry.try_get<Position>(entity);
    if (position == nullptr)
        spdlog::error("Trying to move entity {} without position component", static_cast<int>(entity));
    else
    {
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
    auto* coll_ptr = m_registry.try_get<Collider>(out_of_boundaries_event.entity);

    if (coll_ptr != nullptr and coll_ptr->solid) // Resolve collision
    {
        auto& pos = m_registry.get<Position>(out_of_boundaries_event.entity);

        int a_min, a_max, b_min, b_max;
        a_min = static_cast<int>(out_of_boundaries_event.bound_pos.x) + out_of_boundaries_event.bound_coll.x_offset;
        a_max = a_min + out_of_boundaries_event.bound_coll.width;
        b_min = static_cast<int>(pos.x) + coll_ptr->x_offset;
        b_max = b_min + coll_ptr->width;
        // spdlog::info("boundaries {}, {}, {}, {}", a_min, a_max, b_min, b_max);

        if (b_min < a_min)
            pos.x = a_min - coll_ptr->x_offset;
        else if (b_max > a_max)
            pos.x = a_max - coll_ptr->x_offset - coll_ptr->width;

        a_min = static_cast<int>(out_of_boundaries_event.bound_pos.y) + out_of_boundaries_event.bound_coll.y_offset;
        a_max = a_min + out_of_boundaries_event.bound_coll.height;
        b_min = static_cast<int>(pos.y) + coll_ptr->y_offset;
        b_max = b_min + coll_ptr->height;
        if (b_min < a_min)
            pos.y = a_min - coll_ptr->y_offset;
        else if (b_max > a_max)
            pos.y = a_max - coll_ptr->y_offset - coll_ptr->height;
    }
}
