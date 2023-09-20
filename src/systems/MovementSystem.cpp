#include "MovementSystem.hpp"
#include "../Components.hpp"
#include <spdlog/spdlog.h>


void MovementSystem::Update(entt::registry &registry) {
    auto view = registry.view<Position, const Velocity>();

    // use a callback
    view.each([](auto &pos, auto const &vel) {
        pos.x = pos.x + vel.dx;
        pos.y = pos.y + vel.dy;
    });

//    // use an extended callback
//    view.each([](const auto entity, const auto &pos, auto &vel) { /* ... */ });
//
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


void MovementSystem::OnInputEvent(InputEvent input_event) {
    spdlog::info("Received event");
}