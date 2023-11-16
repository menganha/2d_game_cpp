#include "CombatSystem.hpp"

#include "../Colors.hpp"
#include "../Events.hpp"

#include <spdlog/spdlog.h>

CombatSystem::CombatSystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{ registry }
  , m_dispatcher{ dispatcher }
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
            m_registry.destroy(entity);
            m_dispatcher.enqueue<DeathEvent>(entity);
        }
    }
}

void
CombatSystem::OnShootEvent(ShootEvent shoot_event)
{
    auto& position = m_registry.get<Position>(shoot_event.shooter_entity);
    auto  bullet_entity = m_registry.create();
    m_registry.emplace<Position>(bullet_entity, position.x + 3, position.y - 10);
    m_registry.emplace<Velocity>(bullet_entity, 0.f, -6.f);
    m_registry.emplace<SquarePrimitive>(bullet_entity, 4, 7, Colors::BLACK);
    m_registry.emplace<Collider>(bullet_entity, 4, 7, 0, 0, false);
    m_registry.emplace<Weapon>(bullet_entity, 5);
}

void
CombatSystem::OnCollisionEvent(CollisionEvent collision_event)
{

    if (m_registry.all_of<Health>(collision_event.entity_a) and m_registry.all_of<Weapon>(collision_event.entity_b))
    {
        HandleDealingDamage(m_registry.get<Health>(collision_event.entity_a),
                            collision_event.entity_a,
                            m_registry.get<Weapon>(collision_event.entity_b),
                            collision_event.entity_b);
    }

    if (m_registry.all_of<Weapon>(collision_event.entity_a) and m_registry.all_of<Health>(collision_event.entity_b))
    {
        HandleDealingDamage(m_registry.get<Health>(collision_event.entity_b),
                            collision_event.entity_b,
                            m_registry.get<Weapon>(collision_event.entity_a),
                            collision_event.entity_a);
    }
}

void
CombatSystem::OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event)
{
    if (m_registry.all_of<Weapon>(out_of_boundaries_event.entity))
    {
        spdlog::info("Destroying entity {}. Reason: Out of boundaries",
                     static_cast<int>(out_of_boundaries_event.entity));
        m_registry.destroy(out_of_boundaries_event.entity);
    }
}

void
CombatSystem::HandleDealingDamage(Health& health, entt::entity health_ent, Weapon weapon, entt::entity weapont_ent)
{
    health.points -= weapon.power;
    m_dispatcher.enqueue<HealthEvent>(health_ent, health.points);
    if (weapon.disposable)
        m_registry.destroy(weapont_ent);
}
