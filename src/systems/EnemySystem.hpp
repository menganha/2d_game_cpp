#pragma once

#include "../Enemy.hpp"

#include <entt/fwd.hpp>

//
// It takes care (in the moment) of the enemy loading for each level and some enemy behaviour
//

struct EnemyEntry
{
  EnemyBreed breed;
  int        delay;
  int        pos_x;
  int        pos_y;
};

class EnemySystem
{

public:
  EnemySystem(entt::registry& registry, entt::dispatcher& dispatcher);
  void Update(entt::entity player_entity);
  void SetEnemyList(const std::vector<EnemyEntry>& enemy_list);

private:
  entt::registry&         m_registry;
  entt::dispatcher&       m_dispatcher;
  std::vector<EnemyEntry> m_enemy_list_to_dispatch;
  int  m_level_counter; // FIXME: Could this cause an overflow at some point? Levels should not be so large I guess (?)
  bool m_level_ended_event_triggered;
};

