#include "Script.hpp"

MoveEntityScript::MoveEntityScript(entt::registry& registry, entt::entity entity, int x, int y, int ticks_it_takes)
  : m_finished{false}
  , m_ticks_it_takes{ticks_it_takes}
  , m_ticks_passed{0}
  , m_final_position{static_cast<float>(x), static_cast<float>(y)}
  , m_initial_position{registry.get<Position>(entity)}
  , m_position{registry.get<Position>(entity)}
{}

void
MoveEntityScript::Update()
{
  m_ticks_passed += 1;

  m_position.x = (m_final_position.x - m_initial_position.x) * m_ticks_passed / m_ticks_it_takes + m_initial_position.x;
  m_position.y = (m_final_position.y - m_initial_position.y) * m_ticks_passed / m_ticks_it_takes + m_initial_position.y;

  if (m_ticks_passed >= m_ticks_it_takes) {
    m_position.x = m_final_position.x;
    m_position.y = m_final_position.y;
    m_finished = true;
  }
}
