#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include "Gamepad.hpp"

struct Position {
    float x;
    float y;
};

struct Renderable {
    float x;
    float y;
};

struct Velocity {
    float dx;
    float dy;
};

struct Color {
    SDL_Color color;
};

class GameScene {
private:
    static constexpr SDL_Color WHITE{0xAB, 0xB2, 0xBF, 0xFF};
    bool m_is_running;
    Gamepad m_gamepad;
    SDL_Renderer *m_renderer;
    entt::registry m_registry;

    void update_render_system();

    void update_movement_system();

public:
    explicit GameScene(SDL_Renderer *renderer);

    [[nodiscard]] bool IsRunning() const;

    void Run();

};


#endif //GAMESCENE_H_
