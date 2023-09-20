#ifndef INC_2D_CPP_ECS_GAME_GAMESCENE_H_HPP
#define INC_2D_CPP_ECS_GAME_GAMESCENE_H_HPP

#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include "Gamepad.hpp"

class GameScene {
private:
    static constexpr SDL_Color WHITE{0xAB, 0xB2, 0xBF, 0xFF};
    static constexpr SDL_Color RED{0xFF, 0x00, 0x00, 0x00};
    bool m_is_running;
    Gamepad m_gamepad;
    SDL_Renderer *m_renderer;
    entt::registry m_registry;
    entt::dispatcher m_dispatcher;

    void update_render_system();


public:
    explicit GameScene(SDL_Renderer *renderer);

    [[nodiscard]] bool IsRunning() const;

    void Run();

};


#endif //INC_2D_CPP_ECS_GAME_GAMESCENE_H_HPP
