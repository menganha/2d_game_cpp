#include "MovementSystem.hpp"
#include "../Components.hpp"
#include <spdlog/spdlog.h>


MovementSystem::MovementSystem(entt::registry& registry) : m_registry{registry} {
};

void MovementSystem::Update() {
    auto view = m_registry.view<Position, const Velocity>();

    // use a callback
    view.each([](auto& pos, auto const& vel) {
        pos.x = pos.x + vel.dx;
        pos.y = pos.y + vel.dy;
    });

//    // use a range-for
//    for (auto [entity, pos, vel]: view.each()) {
//        // ...
//    }
//
//    // use forward iterators and get only the components of interest
//    for (auto entity: view) {
//        auto &vel = view.get<Velocity>(entity);
//        // ...
//    }
}


void MovementSystem::OnDirectionalButtonEvent(DirectionalButtonEvent directional_button_event) {
    auto view = m_registry.view<const Movable, Velocity>();
    // use a callback
    view.each([directional_button_event](const auto& movable, auto& vel) {
        vel.dx = directional_button_event.right - directional_button_event.left;
        vel.dy = directional_button_event.down - directional_button_event.up;
    });
}