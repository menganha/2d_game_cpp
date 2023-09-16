#include <iostream>
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

struct position {
    float x;
    float y;
};

struct velocity {
    float dx;
    float dy;
};

struct color {
    SDL_Color color;
};

void update_render_engine(entt::registry &registry, SDL_Renderer *renderer) {
    auto view = registry.view<const position, const color>();
    view.each([renderer](auto &pos, const auto &col) {
        SDL_SetRenderDrawColor(renderer, col.color.r, col.color.g, col.color.b, col.color.a);
        SDL_Rect rectangle{static_cast<int>(pos.x), static_cast<int>(pos.y), 10, 10};
        SDL_RenderFillRect(renderer, &rectangle);
    });

    // use a range-for
//    for (auto [entity, pos, color]: view.each()) {
//        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
//        SDL_Rect rectangle{pos.x, pos.y, 4, 4};
//        SDL_RenderFillRect(renderer, &rectangle);
//    }
}

void update_entities(entt::registry &registry) {
    auto view = registry.view<position, const velocity>();

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
        auto &vel = view.get<velocity>(entity);
        // ...
    }
}

bool is_running() {
    SDL_Event sdlEvent;
    bool is_running{true};
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                }
                break;
        }
    }
    return is_running;
}


int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::string err_msg = "SDL could not initialize! SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    SDL_Window *window_ = SDL_CreateWindow("Yet Another Tetris Clone",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           800,
                                           600,
                                           SDL_WINDOW_SHOWN);
    if (window_ == nullptr) {
        std::string err_msg = "Window could not be created! SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    SDL_Renderer *renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer_ == nullptr) {
        std::string err_msg = "Renderer could not be created SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    entt::registry registry;
    SDL_Color WHITE{0xAB, 0xB2, 0xBF, 0xFF};

    for (auto i = 0u; i < 15u; ++i) {
        const auto entity = registry.create();
        registry.emplace<position>(entity, 20, i * 20.f);
        registry.emplace<color>(entity, WHITE);
        if (i % 2 == 0) { registry.emplace<velocity>(entity, i * .1f, 0); }
    }
    while (is_running()) {
        SDL_SetRenderDrawColor(renderer_, 21, 21, 21, 255);
        SDL_RenderClear(renderer_);
        update_entities(registry);
        update_render_engine(registry, renderer_);
        SDL_RenderPresent(renderer_);
    }

    return 0;
}
