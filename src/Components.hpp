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
  int z{0}; // On 2 games this represent the order of drawing. Larger integers mean to draw last (closer to the camera)
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

struct FullText
{
  std::string string;
  std::string font_id;
  SDL_Color   color;
};

struct TypedText
{
  FullText full_text;
  int      type_delay = 5; // amount of frames to print each character
  int      index = 0;      // Cursor index, i.e., under which character we are in the list
  int      pos_x = 0;      // relative position of the current glyph
  int      pos_y = 0;
  int      delay_counter = 0;
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
  std::string name;                   // Id of the SDL texture to be rendered
  Size        size;                   // Destination square size in pixels
  SDL_Rect    subsection{0, 0, 0, 0}; // Rect of the texture subsection we want to show
  SDL_Color   color_mod{0, 0, 0, 0};  // Last byte != 0 would represent if one wants to modify the color
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

struct Death
{}; // Empty tag component signaling that the entity is to be destroyed

