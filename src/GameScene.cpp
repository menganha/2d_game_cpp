#include "GameScene.hpp"

#include "Components.hpp"
#include "systems/MovementSystem.hpp"
#include "Events.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

void GameScene::update_render_system() {
    auto view = m_registry.view<const Position, const Color>();
    view.each([this](auto &pos, const auto &col) {
        SDL_SetRenderDrawColor(this->m_renderer, col.color.r, col.color.g, col.color.b, col.color.a);
        SDL_Rect rectangle{static_cast<int>(pos.x), static_cast<int>(pos.y), 10, 10};
        SDL_RenderFillRect(this->m_renderer, &rectangle);
    });

}

GameScene::GameScene(SDL_Renderer *renderer) :
        m_is_running{true},
        m_gamepad{},
        m_renderer{renderer},
        m_registry{},
        m_dispatcher{} {


    // creates a "player" entity
    const auto player_entity = m_registry.create();
    m_registry.emplace<Position>(player_entity, 20.0f, 0.0f);
    m_registry.emplace<Color>(player_entity, RED);
    m_registry.emplace<Velocity>(player_entity, 0.0f, 0.0f);
    m_registry.emplace<Movable>(player_entity);

    for (int i = 1; i < 15; ++i) {
        const auto entity = m_registry.create();
        m_registry.emplace<Position>(entity, 20.0f, i * 20.0f);
        m_registry.emplace<Color>(entity, WHITE);
        spdlog::debug("Entity number {}, {}%2 velocity x axis {}", i, i, static_cast<float>(i) * .1f);
        if (i % 2 == 0) { m_registry.emplace<Velocity>(entity, i * .1f, 0.0f); }
    }
}

bool GameScene::IsRunning() const {
    return m_is_running;
}

void GameScene::Run() {
    // We should move this to the constructor
    MovementSystem movement_system{m_registry};
    m_dispatcher.sink<DirectionalButtonEvent>().connect<&MovementSystem::OnDirectionalButtonEvent>(movement_system);

    while (IsRunning()) {

        // SDL stuff
        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
            switch (sdl_event.type) {
                case SDL_QUIT:
                    m_is_running = false;
                    break;
            }
        }
        auto keyboard_state = SDL_GetKeyboardState(nullptr);
        m_gamepad.Update(keyboard_state);

        // Process event inputs translated. for example!
        if (m_gamepad.IsDirectionalButtonEvent()) {
            m_dispatcher.trigger(DirectionalButtonEvent{
                m_gamepad[Gamepad::UP], m_gamepad[Gamepad::DOWN],
                m_gamepad[Gamepad::LEFT], m_gamepad[Gamepad::RIGHT]}
            );
        }

        // Clear screen
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_renderer);
        // Receive input

        // Game Logic
        movement_system.Update();
        update_render_system();

        // Present
        SDL_RenderPresent(m_renderer);
    }
}