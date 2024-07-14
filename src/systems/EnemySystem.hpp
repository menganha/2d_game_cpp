#pragma once

#include <entt/fwd.hpp>
#include "../Enemy.hpp"

//
// It takes care (in the moment) of the enemy loading for each level and some enemy behaviour
//
class EnemySystem
{

public:
  EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher);
  void Update(entt::entity player_entity);
  void SetEnemyList(const std::vector<EnemyEntry>& enemy_list);

private:

  entt::registry&   m_registry;
  entt::dispatcher& m_dispatcher;
  std::vector<EnemyEntry> m_enemy_list_to_dispatch;
  int m_level_counter; // FIXME: Could this cause an overflow at some point? Levels should not be so large I guess (?)
};

