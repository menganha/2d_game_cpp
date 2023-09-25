#include "GameScene.hpp"

#include <spdlog/spdlog.h>
#include <cstdlib>
#include "Components.hpp"
#include "Events.hpp"


GameScene::GameScene(const char* title, int screen_width, int screen_height, Uint32 flags) :
        m_gamepad{},
        m_window{},
        m_registry{},
        m_dispatcher{},
        m_movement_system{m_registry, screen_width, screen_height},
        m_collision_system{m_registry, m_dispatcher},
        m_render_system{m_registry} {

    // Initializes SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::string err_msg = "SDL could not initialize! SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    // Creates window
    m_window.Create(title, screen_width, screen_height, flags);

    // creates a "player" entity
    const auto player_entity = m_registry.create();
    m_registry.emplace<Position>(player_entity, 20, 0);
    m_registry.emplace<Renderable>(player_entity, 10, 10, RED);
    m_registry.emplace<Velocity>(player_entity, 0, 0);
    m_registry.emplace<Movable>(player_entity);
    m_registry.emplace<Collider>(player_entity, 10, 10);
    spdlog::debug("Created entity with id {} at x={}, y={}", static_cast<int>(player_entity), 20, 0);

    for (int i = 1; i < 30; ++i) {
        const auto entity = m_registry.create();
        m_registry.emplace<Position>(entity, rand() % (screen_width - 10), rand() % (screen_height - 10));
        m_registry.emplace<Collider>(entity, 10, 10);
        m_registry.emplace<Renderable>(entity, 10, 10, WHITE);
        if (i == 2) {
            m_registry.emplace<Movable>(entity);
        }
        if (i % 2 == 0) { m_registry.emplace<Velocity>(entity, rand() % 4, rand() % 4); }
        spdlog::debug("Created entity with id {} at x={}, y={}", static_cast<int>(entity), 20, i * 20);
    }

    // initializes the systems
    m_render_system.Init(m_window.GetRenderer());

    // Sets some event listeners
    m_dispatcher.sink<DirectionalButtonEvent>().connect<&MovementSystem::OnDirectionalButtonEvent>(m_movement_system);
    m_dispatcher.sink<CollisionEvent>().connect<&CollisionSystem::OnCollisionEvent>(m_collision_system);

}

void GameScene::Run() {
    while (m_window.IsOpen()) {
        ProcessEvents();
        UpdateLogic();
        Render();
    }
}


void GameScene::ProcessEvents() {

    // Process events from the SDL window
    m_window.ProcessEvents();

    // Keyboard processing
    auto keyboard_state = SDL_GetKeyboardState(nullptr);
    m_gamepad.Update(keyboard_state);

    if (m_gamepad.IsDirectionalButtonEvent()) {
        m_dispatcher.trigger(DirectionalButtonEvent{
                m_gamepad.GetKeyState(Gamepad::UP), m_gamepad.GetKeyState(Gamepad::DOWN),
                m_gamepad.GetKeyState(Gamepad::LEFT), m_gamepad.GetKeyState(Gamepad::RIGHT)}
        );
    }

    // Dispatch any game event accumulated from the previous frame here
    m_dispatcher.update();

}

void GameScene::UpdateLogic() {
    m_movement_system.Update();
    m_collision_system.Update();
}

void GameScene::Render() {

    SDL_SetRenderDrawColor(m_window.GetRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(m_window.GetRenderer());

    m_render_system.Update();

    SDL_RenderPresent(m_window.GetRenderer());
}