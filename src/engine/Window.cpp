#include "Window.hpp"

#include <stdexcept>

Window::Window(const char* title, int screen_width, int screen_height, Uint32 flags)
  : m_window{ nullptr }
  , m_renderer{ nullptr }
  , m_is_open{ false }
  , m_event{}
{

    m_window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, flags);
    if (m_window == nullptr)
    {
        std::string err_msg = "SDL Error creating Window: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
    {
        std::string err_msg = "SDL Error creating Renderer: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }
    m_is_open = true;
}

Window::~Window()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

Window&
Window::operator=(Window&& window) noexcept
{
    // Self-assignment detection
    if (&window == this)
        return *this;

    // Release any resource we're holding
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    // Transfer ownership
    m_window = window.m_window;
    m_renderer = window.m_renderer;
    m_is_open = window.m_is_open;
    m_event = window.m_event;

    window.m_window = nullptr; // leave objects in a valid state. Otherwise we are left with dangling pointers
    window.m_renderer = nullptr;

    return *this;
}

void
Window::ProcessEvents()
{
    while (SDL_PollEvent(&m_event))
    {
        switch (m_event.type)
        {
            case SDL_QUIT:
                Close();
                break;
        }
    }
}
