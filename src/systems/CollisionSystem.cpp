#include "CollisionSystem.hpp"
#include <spdlog/spdlog.h>

CollisionSystem::CollisionSystem(entt::registry& registry, entt::dispatcher& dispatcher) :
        m_registry{registry},
        m_dispatcher{dispatcher} {};

void CollisionSystem::Update() {

    auto kinematic_colliders = m_registry.view<Position, Velocity, Collider>();
    auto static_colliders = m_registry.view<Position, Collider>(entt::exclude<Velocity>);


    for (auto entity_a: kinematic_colliders) {
        auto& collider_a = kinematic_colliders.get<Collider>(entity_a);
        auto& position_a = kinematic_colliders.get<Position>(entity_a);

        for (auto entity_b: static_colliders) {
            auto& collider_b = static_colliders.get<Collider>(entity_b);
            auto& position_b = static_colliders.get<Position>(entity_b);

            if (HasCollided(position_a, collider_a, position_b, collider_b)) {
                m_dispatcher.enqueue<CollisionEvent>(entity_a, entity_b);
            }
        }
    }
}

bool CollisionSystem::HasCollided(Position pos_a, Collider coll_a, Position pos_b, Collider coll_b) {
    // AABB collision check. Copied from SDL
    int a_min, a_max, b_min, b_max;

    // Horizontal intersection
    a_min = pos_a.x;
    a_max = a_min + coll_a.width;
    b_min = pos_b.x;
    b_max = b_min + coll_b.width;

    if (b_min > a_min)
        a_min = b_min;
    if (b_max < a_max)
        a_max = b_max;
    if (a_max <= a_min)
        return false;

    // Vertical intersection
    a_min = pos_a.y;
    a_max = a_min + coll_a.height;
    b_min = pos_b.y;
    b_max = b_min + coll_b.height;
    if (b_min > a_min)
        a_min = b_min;
    if (b_max < a_max)
        a_max = b_max;
    if (a_max <= a_min)
        return false;

    return true;

}

void CollisionSystem::OnCollisionEvent(CollisionEvent collision_event) {
    auto & renderable_a = m_registry.get<Renderable>(collision_event.entity_a);
    auto & renderable_b = m_registry.get<Renderable>(collision_event.entity_b);
    renderable_a.color = SDL_Color{0xE6, 0xE6, 0x00, 0x00};
    renderable_b.color = SDL_Color{0xE6, 0xE6, 0x00, 0x00};
    spdlog::info("Entity {} has collided with entity {}",
                 static_cast<std::uint32_t >(collision_event.entity_a),
                 static_cast<std::uint32_t >(collision_event.entity_b));
}