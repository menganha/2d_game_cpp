#include "Components.hpp"
#include "Enemy.hpp"

#include <entt/fwd.hpp>

namespace entities {

constexpr static int   PLAYER_INITIAL_HEALTH = 10;
constexpr static float PLAYER_VELOCITY = 2.0f;
constexpr static int   PLAYER_INVINCIBILITY_TIME = 120;
constexpr static float PLAYER_BULLET_VELOCITY = -6.0f;
constexpr static int   PLAYER_BULLET_SIZE = 5;
constexpr static int   PLAYER_BULLET_POWER = 10;

constexpr static float ENEMY_BULLET_VELOCITY = 6.0f;
constexpr static int   ENEMY_BULLET_SIZE = 5;
constexpr static int   ENEMY_BULLET_POWER = 5;

entt::entity
CreatePlayer(entt::registry& registry);

void
PlayerShoot(entt::entity player_entity, entt::registry&);

entt::entity
CreateBullet(entt::registry& registry, const Position& position, const Velocity& velocity, int size, int power);

entt::entity
CreateEnemy(entt::registry& registry, EnemyBreed enemy_breed, int pos_x, int pos_y);

void
EnemyShoot(entt::registry& registry, const Position& enemy_position, const Velocity& velocity);

entt::entity
CreateCamera(entt::registry& registry);

};
