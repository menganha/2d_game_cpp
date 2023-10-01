#include "CombatSystem.hpp"
#include "../Components.hpp"

CombatSystem::CombatSystem(entt::registry& registry) : m_registry{registry} {}

void CombatSystem::Update() {
    auto view = m_registry.view<Health>();
    for (auto entity: view) {
        auto health = view.get<Health>(entity);
        if (health.points <= 0) {
            m_registry.destroy(entity);
        }
    }
}

void CombatSystem::OnShootButtonEvent(ShootEvent shoot_event) {
    auto& position = m_registry.get<Position>(shoot_event.shooter_entity);
    auto bullet_entity = m_registry.create();
    m_registry.emplace<Position>(bullet_entity, position.x + 3, position.y - 10);
    m_registry.emplace<Velocity>(bullet_entity, 0, -6);
    SDL_Color black_color{0x00, 0x00, 0x00, 0x00};
    m_registry.emplace<Renderable>(bullet_entity, 4, 5, black_color);
    m_registry.emplace<Collider>(bullet_entity, 4, 5, 0, 0);
    m_registry.emplace<Misile>(bullet_entity, 5);
}

void CombatSystem::OnCollisionEvent(CollisionEvent collision_event) {
    if (m_registry.all_of<Health>(collision_event.entity_a) and m_registry.all_of<Misile>(collision_event.entity_b)) {
        auto& health = m_registry.get<Health>(collision_event.entity_a);
        auto weapon = m_registry.get<Misile>(collision_event.entity_b);
        health.points -= weapon.power;
    } else if (m_registry.all_of<Misile>(collision_event.entity_a) and m_registry.all_of<Health>(collision_event.entity_b)) {
        auto& health = m_registry.get<Health>(collision_event.entity_b);
        auto weapon = m_registry.get<Misile>(collision_event.entity_a);
        health.points -= weapon.power;
    } else {
        return;
    }
}

void CombatSystem::OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event) {
    auto missile = m_registry.try_get<Misile>(out_of_boundaries_event.entity);
    if (missile != nullptr){
        m_registry.destroy(out_of_boundaries_event.entity);
    }
}