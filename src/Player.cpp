#include "Player.hpp"

#include "Colors.hpp"
#include "engine/Log.hpp"


Player::Player(entt::registry& registry) : m_registry{registry}, m_entity{entt::null} {}

void
Player::Create()
{
    m_entity = m_registry.create();
    LINFO("Creating player entity with id %i", static_cast<int>(m_entity));
    m_registry.emplace<Position>(m_entity, 402.f, 500.f);
    m_registry.emplace<SquarePrimitive>(m_entity, 10, 10, Colors::RED);
    m_registry.emplace<Velocity>(m_entity, 0.f, 0.f);
    m_registry.emplace<Collider>(m_entity, 10, 10, 0, 0, true);
    m_registry.emplace<Health>(m_entity, PLAYER_INITIAL_HEALTH, 0, PLAYER_INVINCIBILITY_TIME);
}

Position&
Player::GetPosition() const
{
    return m_registry.get<Position>(m_entity);
}

//TODO: Create a generic function to create bullets
void
Player::Shoot()
{
    auto bullet_entity = m_registry.create();
    LDEBUG("Creating player bullet with id %i", static_cast<int>(bullet_entity));
    auto position = GetPosition();
    m_registry.emplace<Position>(bullet_entity, position.x + 3, position.y - 10);
    m_registry.emplace<Velocity>(bullet_entity, 0.f, -6.f);
    m_registry.emplace<SquarePrimitive>(bullet_entity, 4, 7, Colors::BLACK);
    m_registry.emplace<Collider>(bullet_entity, 4, 7, 0, 0, false);
    m_registry.emplace<Weapon>(bullet_entity, 5, true);
}

