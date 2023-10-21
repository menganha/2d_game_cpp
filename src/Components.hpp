#ifndef INC_2D_CPP_ECS_GAME_COMPONENTS_HPP
#define INC_2D_CPP_ECS_GAME_COMPONENTS_HPP

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

using namespace entt::literals;

// TODO: Maybe not necessary. We will keep track of the player with its entity ID
using Movable = entt::tag<"Movable"_hs>;

struct Position
{
    float x;
    float y;
};

struct Velocity
{
    float dx;
    float dy;
};

class Counter
{
  public:
    Counter() = default;
    Counter(int goal_count) : m_goal_count{ goal_count }, m_current_count{ 0 } {}
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
    int       height;
    int       width;
    SDL_Color color;
};

struct Collider
{
    int  width;
    int  height;
    int  x_offset = 0;
    int  y_offset = 0;
    bool solid = false; // Means it cannot go over the external boundary
};

struct Health
{
    int points;
};

struct Misile
{
    int power;
};

#endif // INC_2D_CPP_ECS_GAME_COMPONENTS_HPP
