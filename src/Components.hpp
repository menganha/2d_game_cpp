#pragma once

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <string>
#include <cstring>

using namespace entt::literals;

struct Position
{
    float x;
    float y;
};

struct Text
{
    std::string text;
    std::string font_id;
    SDL_Color   color;
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
    Counter(int goal_count) : m_goal_count{goal_count}, m_current_count{0} {}
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

// Primitive square shape of SDL
struct SquarePrimitive
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

// Something that make damage
struct Weapon
{
    int  power;
    bool disposable = true;
};

enum class EnemyType : int
{
    NOTYPE = 0,
    SIMPLE, // Just moves Down
    PARAB,
};


struct Enemy
{
    EnemyType type;
    float     lifetime; // Keeps track of the time alive for deriving movement
    Position  reach;
    float     parametric_vel;
    float     spread;
};
