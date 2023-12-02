#pragma once

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <cmath>
#include <string>

using namespace entt::literals;

struct Velocity
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
T UnitDifference(T vector1, T vector2){
    auto x = vector1.x - vector2.x;
    auto y = vector1.y = vector2.y;
    auto mod = std::sqrt(x*x + y*y);
    return T{x/mod, y/mod}; 
}

struct Text
{
    std::string text;
    std::string font_id;
    SDL_Color   color;
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
    Size      size;
    SDL_Color color;
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
    Size offset;
    bool solid = false; // Means it cannot go over the external boundary
};

struct Health
{
    int points;
};

// Something that can make damage
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
