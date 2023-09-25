#ifndef INC_2D_CPP_ECS_GAME_WINDOW_HPP
#define INC_2D_CPP_ECS_GAME_WINDOW_HPP

#include <SDL2/SDL.h>

class Window {
public:
    Window();

    ~Window();

    void Create(const char* title, int screen_width, int screen_height, Uint32 flags);

    [[nodiscard]] bool IsOpen() const { return m_is_open; }

    void Close() { m_is_open = false; };

    void ProcessEvents();

    [[nodiscard]] SDL_Renderer* GetRenderer() const { return m_renderer; };

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_is_open;
    SDL_Event m_event;
};


#endif //INC_2D_CPP_ECS_GAME_WINDOW_HPP
