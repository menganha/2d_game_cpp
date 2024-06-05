#include "CombatSystem.hpp"

#include "../Events.hpp"
#include "../Enemy.hpp"

#include <spdlog/spdlog.h>

CombatSystem::CombatSystem(entt::registry& registry, entt::dispatcher& dispatcher)
  : m_registry{ registry }
  , m_dispatcher{ dispatcher }
{
}

void
CombatSystem::Update()
{
  // TODO: should we have another system for just he health and resolving the death of an entity?
  auto view = m_registry.view<Health>();
  for (auto entity : view) {
    auto& health = view.get<Health>(entity);
    if (health.points <= 0) {
      m_dispatcher.enqueue<DestroyEvent>(entity);
    }
    if (health.invincibility_timer > 0) {
      health.invincibility_timer -= 1;
    }
  }
}

////TODO: Put this in the enemy system and trigger immediatly. Create a generic function to create bullets
//void
//CombatSystem::OnShootEvent(BulletEvent bullet_event)
//{
//  auto bullet_entity = m_registry.create();
//  spdlog::trace("Creating bullet entity {}", static_cast<int>(bullet_entity));
//  m_registry.emplace<Position>(bullet_entity, bullet_event.position.x, bullet_event.position.y);
//  m_registry.emplace<Velocity>(bullet_entity, bullet_event.velocity.x, bullet_event.velocity.y);
//  m_registry.emplace<SquarePrimitive>(
//    bullet_entity, bullet_event.collider_size.x, bullet_event.collider_size.y, bullet_event.color);
//  m_registry.emplace<Collider>(bullet_entity, bullet_event.collider_size.x, bullet_event.collider_size.y, 0, 0, false);
//  m_registry.emplace<Weapon>(bullet_entity, 5);
//}

void
CombatSystem::OnCollisionEvent(CollisionEvent collision_event)
{
  if (m_registry.all_of<Health>(collision_event.entity_a) and m_registry.all_of<Weapon>(collision_event.entity_b)) {
    HandleDealingDamage(m_registry.get<Health>(collision_event.entity_a),
                        collision_event.entity_a,
                        m_registry.get<Weapon>(collision_event.entity_b),
                        collision_event.entity_b);
  }

  if (m_registry.all_of<Weapon>(collision_event.entity_a) and m_registry.all_of<Health>(collision_event.entity_b)) {
    HandleDealingDamage(m_registry.get<Health>(collision_event.entity_b),
                        collision_event.entity_b,
                        m_registry.get<Weapon>(collision_event.entity_a),
                        collision_event.entity_a);
  }
}

// Removes entities with weapon components if they are out the grid
void
CombatSystem::OnOutOfBoundariesEvent(OutOfBoundariesEvent out_of_boundaries_event)
{
  if (m_registry.all_of<Weapon>(out_of_boundaries_event.entity)) {
    m_dispatcher.enqueue(DestroyEvent{ out_of_boundaries_event.entity });
  }
}

void
CombatSystem::HandleDealingDamage(Health& health, entt::entity health_ent, Weapon weapon, entt::entity weapon_ent)
{
  // Ignores friendly fire
  if (m_registry.all_of<Enemy>(health_ent) and weapon.from_enemy)
    return;

  if (health.invincibility_timer == 0) {
    health.points -= weapon.power;
    health.invincibility_timer = health.max_invincibility_time;
    m_dispatcher.enqueue<DamageEvent>(health_ent, health.points);
    if (weapon.disposable)
      m_dispatcher.enqueue<DestroyEvent>(weapon_ent);
  }
}
