#include "GameScene.hpp"

#include <SDL2/SDL.h>
#include "Gamepad.hpp"

void GameScene::update_render_system() {
    auto view = m_registry.view<const Position, const Color>();
    view.each([this](auto &pos, const auto &col) {
        SDL_SetRenderDrawColor(this->m_renderer, col.color.r, col.color.g, col.color.b, col.color.a);
        SDL_Rect rectangle{static_cast<int>(pos.x), static_cast<int>(pos.y), 10, 10};
        SDL_RenderFillRect(this->m_renderer, &rectangle);
    });

}

void GameScene::update_movement_system() {
    auto view = m_registry.view<Position, const Velocity>();

    // use a callback
    view.each([](auto &pos, auto const &vel) {
        pos.x = pos.x + vel.dx;
        pos.y = pos.y + vel.dy;
    });

    // use an extended callback
    view.each([](const auto entity, const auto &pos, auto &vel) { /* ... */ });

    // use a range-for
    for (auto [entity, pos, vel]: view.each()) {
        // ...
    }

    // use forward iterators and get only the components of interest
    for (auto entity: view) {
        auto &vel = view.get<Velocity>(entity);
        // ...
    }
}

GameScene::GameScene(SDL_Renderer *renderer) :
        m_renderer{renderer},
        m_is_running{true},
        m_registry{},
        m_gamepad{} {

    for (auto i = 0u; i < 15u; ++i) {
        const auto entity = m_registry.create();
        m_registry.emplace<Position>(entity, 20, i * 20.f);
        m_registry.emplace<Color>(entity, WHITE);
        if (i % 2 == 0) { m_registry.emplace<Velocity>(entity, i * .1f, 0); }
    }
}

bool GameScene::IsRunning() const {
    return m_is_running;
}

void GameScene::Run() {
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
        if (m_gamepad.IsButtonReleased(Gamepad::UP)) {
            // do something
        }

        // Clear screen
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_renderer);
        // Receive input

        // Game Logic
        update_movement_system();
        update_render_system();

        // Present
        SDL_RenderPresent(m_renderer);
    }
}