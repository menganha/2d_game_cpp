#include "CombatSystem.hpp"

#include "../Colors.hpp"
#include "../Components.hpp"
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
CombatSystem::OnShootButtonEvent(ShootEvent shoot_event)
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
        auto& health = m_registry.get<Health>(collision_event.entity_a);
        auto  weapon = m_registry.get<Weapon>(collision_event.entity_b);
        health.points -= weapon.power;
        if (weapon.disposable)
            m_registry.destroy(collision_event.entity_b);
    }

    if (m_registry.all_of<Weapon>(collision_event.entity_a) and m_registry.all_of<Health>(collision_event.entity_b))
    {
        auto& health = m_registry.get<Health>(collision_event.entity_b);
        auto  weapon = m_registry.get<Weapon>(collision_event.entity_a);
        health.points -= weapon.power;
        if (weapon.disposable)
            m_registry.destroy(collision_event.entity_a);
    }
}

void
CombatSystem::OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event)
{
    if (m_registry.all_of<Weapon>(out_of_boundaries_event.entity))
    {
        spdlog::info("Destroying entity {}. Reason: Out of boundaries", static_cast<int>(out_of_boundaries_event.entity));
        m_registry.destroy(out_of_boundaries_event.entity);
    }
}

template<typename Ta, typename Tb>
std::tuple<Ta*, Tb*>
CombatSystem::try_signature(entt::entity entity_a, entt::entity entity_b)
{
    auto* component_a = m_registry.try_get<Ta>(entity_a);
    auto* component_b = m_registry.try_get<Tb>(entity_b);
    if (component_a and component_b)
    {
        return { component_a, component_b };
    }
    else
    {
        auto* component_a = m_registry.try_get<Ta>(entity_b);
        auto* component_b = m_registry.try_get<Tb>(entity_a);
        return { component_a, component_b };
    }
}
