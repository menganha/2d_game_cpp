#ifndef INC_2D_CPP_ECS_GAME_GAMESCENE_H_HPP
#define INC_2D_CPP_ECS_GAME_GAMESCENE_H_HPP

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include "Gamepad.hpp"
#include "Window.hpp"

#include "systems/CollisionSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/CombatSystem.hpp"

class GameScene {

public:
    static constexpr SDL_Color WHITE{0xAB, 0xB2, 0xBF, 0xFF};
    static constexpr SDL_Color RED{0xFF, 0x00, 0x00, 0x00};
    static constexpr SDL_Color GREEN{0x00, 0xFF, 0x00, 0xFF};

    GameScene(const char* title, int screen_width, int screen_height, Uint32 flags);

    void Run();

private:
    void ProcessEvents();

    void UpdateLogic();

    void Render();

    Gamepad          m_gamepad;
    Window           m_window;
    entt::entity     m_player_entity;
    entt::registry   m_registry;
    entt::dispatcher m_dispatcher;
    MovementSystem   m_movement_system;
    CollisionSystem  m_collision_system;
    CombatSystem     m_combat_system;
    RenderSystem     m_render_system;

};


#endif //INC_2D_CPP_ECS_GAME_GAMESCENE_H_HPP