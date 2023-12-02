#include "CombatSystem.hpp"

#include "../Events.hpp"

#include <spdlog/spdlog.h>

CombatSystem::CombatSystem(entt::registry& registry, entt::dispatcher& dispatcher)
    : m_registry{registry}
    , m_dispatcher{dispatcher}
{}

void
CombatSystem::Update()
{
    // TODO: should we have another system for just he health and resolving the death of an entity?
    auto view = m_registry.view<Health>();
    for (auto entity : view)
    {
        auto health = view.get<Health>(entity);
        if (health.points <= 0)
        {
            m_dispatcher.enqueue<DestroyEvent>(entity);
        }
    }
}

void
CombatSystem::OnShootEvent(BulletEvent bullet_event)
{
    auto bullet_entity = m_registry.create();
    spdlog::debug("Creating bullet entity {}", static_cast<int>(bullet_entity));
    m_registry.emplace<Position>(bullet_entity, bullet_event.position.x, bullet_event.position.y);
    m_registry.emplace<Velocity>(bullet_entity, bullet_event.velocity.x, bullet_event.velocity.y);
    m_registry.emplace<SquarePrimitive>(
      bullet_entity, bullet_event.collider_size.x, bullet_event.collider_size.y, bullet_event.color
    );
    m_registry.emplace<Collider>(bullet_entity, bullet_event.collider_size.x, bullet_event.collider_size.y, 0, 0, false);
    m_registry.emplace<Weapon>(bullet_entity, 5);
}

void
CombatSystem::OnCollisionEvent(CollisionEvent collision_event)
{

    // Ignore if both are weapons
    if (m_registry.all_of<Weapon>(collision_event.entity_a) and m_registry.all_of<Weapon>(collision_event.entity_b))
        return;

    if (m_registry.all_of<Health>(collision_event.entity_a) and m_registry.all_of<Weapon>(collision_event.entity_b))
    {
        HandleDealingDamage(
          m_registry.get<Health>(collision_event.entity_a),
          collision_event.entity_a,
          m_registry.get<Weapon>(collision_event.entity_b),
          collision_event.entity_b
        );
    }

    if (m_registry.all_of<Weapon>(collision_event.entity_a) and m_registry.all_of<Health>(collision_event.entity_b))
    {
        HandleDealingDamage(
          m_registry.get<Health>(collision_event.entity_b),
          collision_event.entity_b,
          m_registry.get<Weapon>(collision_event.entity_a),
          collision_event.entity_a
        );
    }
}

void
CombatSystem::OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event)
{
    if (m_registry.all_of<Weapon>(out_of_boundaries_event.entity))
    {
        spdlog::info("Destroying entity {}. Reason: Out of boundaries", static_cast<int>(out_of_boundaries_event.entity));
        m_dispatcher.enqueue(DestroyEvent{out_of_boundaries_event.entity});
    }
}

void
CombatSystem::HandleDealingDamage(Health& health, entt::entity health_ent, Weapon weapon, entt::entity weapon_ent)
{
    health.points -= weapon.power;
    m_dispatcher.enqueue<HealthEvent>(health_ent, health.points);
    if (weapon.disposable)
        m_dispatcher.enqueue<DestroyEvent>(weapon_ent);
}
