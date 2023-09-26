#include "MovementSystem.hpp"
#include "../Components.hpp"


MovementSystem::MovementSystem(entt::registry& registry, int screen_width, int screen_height) :
        m_registry{registry},
        m_screen_width{screen_width},
        m_screen_height{screen_height} {};

void MovementSystem::Update() {
    auto view = m_registry.view<Position, Velocity>();

    // use a callback
    view.each([this](auto& pos, auto& vel) {
        pos.x = pos.x + vel.dx;
        pos.y = pos.y + vel.dy;
        if (pos.x > m_screen_width or pos.x < 0) {
            vel.dx = -vel.dx;
        }
        if (pos.y < 0 or pos.y > m_screen_height) {
            vel.dy = -vel.dy;
        }
    });
}


void MovementSystem::OnDirectionalButtonEvent(DirectionalButtonEvent directional_button_event) {
    auto view = m_registry.view<const Movable, Velocity>();
    for (auto entity: view) {
        auto& velocity = view.get<Velocity>(entity);
        velocity.dx = (directional_button_event.right - directional_button_event.left) * 2;
        velocity.dy = (directional_button_event.down - directional_button_event.up) * 2;
    }
}