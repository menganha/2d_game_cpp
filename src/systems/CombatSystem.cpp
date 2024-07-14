#include "CombatSystem.hpp"

#include "../Enemy.hpp"
#include "../Events.hpp"

CombatSystem::CombatSystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{registry}
  , m_dispatcher{dispatcher}
{
}

void
CombatSystem::Update()
{
  auto view = m_registry.view<Health>();
  for (auto entity : view) {
    auto& health = view.get<Health>(entity);
    if (health.invincibility_timer > 0) {
      health.invincibility_timer -= 1;
    }
  }
}

void
CombatSystem::OnCollisionEvent(CollisionEvent collision_event)
{

  // Ignores friendly fire
  if (m_registry.all_of<Enemy>(collision_event.entity_a) and m_registry.all_of<Enemy>(collision_event.entity_b))
    return;

  if (m_registry.all_of<Health>(collision_event.entity_a) and m_registry.all_of<Weapon>(collision_event.entity_b)) {
    HandleDealingDamage(
      m_registry.get<Health>(collision_event.entity_a), collision_event.entity_a,
      m_registry.get<Weapon>(collision_event.entity_b), collision_event.entity_b);
  }

  if (m_registry.all_of<Weapon>(collision_event.entity_a) and m_registry.all_of<Health>(collision_event.entity_b)) {
    HandleDealingDamage(
      m_registry.get<Health>(collision_event.entity_b), collision_event.entity_b,
      m_registry.get<Weapon>(collision_event.entity_a), collision_event.entity_a);
  }
}

// Removes entities with weapon components if they are out the grid
void
CombatSystem::OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event)
{
  if (m_registry.all_of<Weapon>(out_of_boundaries_event.entity)) {
    m_registry.emplace_or_replace<Death>(out_of_boundaries_event.entity);
  }
}

void
CombatSystem::HandleDealingDamage(Health& health, entt::entity health_ent, Weapon weapon, entt::entity weapon_ent)
{
  if (health.invincibility_timer == 0) {
    health.invincibility_timer = health.max_invincibility_time;
    health.points -= weapon.power;
    if (health.points <= 0) {
      m_registry.emplace_or_replace<Death>(health_ent);
    }
    m_dispatcher.enqueue<DamageEvent>(health_ent, health.points);
    if (weapon.disposable) {
      m_registry.emplace_or_replace<Death>(weapon_ent);
    }
  }
}
