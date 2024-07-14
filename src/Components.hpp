#pragma once

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <string>

struct Velocity
{
  float x;
  float y;
};

struct Acceleration
{
  float x;
  float y;
};

struct Position
{
  float x;
  float y;
};

struct Size
{
  int x;
  int y;
};

template<typename T>
float
VecNorm(T vector)
{
  return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}


struct Text
{
  std::string text;
  std::string font_id; // Is the font ID really necessary here? Maybe another component, maybe just the size?
  SDL_Color   color;
};

class Counter
{
public:
  Counter() = default;
  Counter(int goal_count)
    : m_goal_count{goal_count}
    , m_current_count{0}
  {
  }
  void Tick() { m_current_count += 1; }
  bool IsDone() const { return m_current_count >= m_goal_count; }
  void Reset() { m_current_count = 0; }
  int  GetCurrentCount() { return m_current_count; }

private:
  int m_goal_count;
  int m_current_count;
};

struct Renderable
{
  std::string name;
  Size        size;
  Size        subsection_size{0, 0}; // size of the subsection we want to show
  Size        offset{0, 0};          // x-y offset from (0, 0) of the subsection we want to whow
};

// Primitive square shape of SDL
struct SquarePrimitive
{
  Size      size;
  SDL_Color color;
};

struct Collider
{
  Size size;
  Size offset{0, 0};
  bool solid{false}; // Means it cannot go over the external boundary
};

struct Health
{
  int points;
  int invincibility_timer = 0;
  int max_invincibility_time = 0;
};

// Something that can make damage
struct Weapon
{
  int  power;
  bool disposable = true; // Destroyed when dealing damage
};

struct Death {}; // Empty tag component signaling that the entity is to be destroyed

