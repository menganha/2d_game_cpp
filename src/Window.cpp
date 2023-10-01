#include "Window.hpp"
#include <stdexcept>

Window::Window() : m_window{nullptr}, m_renderer{nullptr}, m_is_open{false}, m_event{} {}

Window::~Window() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    m_renderer = nullptr;
    m_is_open = false;
}

void Window::Create(const char* title, int screen_width, int screen_height, Uint32 flags) {
    m_window = SDL_CreateWindow(title,
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                screen_width,
                                screen_height,
                                flags);
    if (m_window == nullptr) {
        std::string err_msg = "SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr) {
        std::string err_msg = "SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }
    m_is_open = true;
}

void Window::ProcessEvents() {
    while (SDL_PollEvent(&m_event)) {
        switch (m_event.type) {
            case SDL_QUIT:
                Close();
                break;
        }
    }
}
