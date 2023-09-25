#ifndef INC_2D_CPP_ECS_GAME_RENDERSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_RENDERSYSTEM_HPP

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

class RenderSystem {

public:
    explicit RenderSystem(entt::registry& registry);
    // We must initialize it only after we have initialized the window with this function, as the loading of the
    // SDL, Creation of a window, can fail etc.
    void Init(SDL_Renderer * renderer);

    void Update();

private:
    entt::registry& m_registry;
    SDL_Renderer* m_renderer;
};


#endif //INC_2D_CPP_ECS_GAME_RENDERSYSTEM_HPP
