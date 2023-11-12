#ifndef INC_2D_CPP_ECS_GAME_RENDERSYSTEM_HPP
#define INC_2D_CPP_ECS_GAME_RENDERSYSTEM_HPP

#include "../AssetManager.hpp"

#include <SDL2/SDL.h>
#include <entt/fwd.hpp>

class RenderSystem
{

  public:
    explicit RenderSystem(entt::registry& registry);
    void Update(const AssetManager& asset_manager, SDL_Renderer* renderer);

  private:
    entt::registry& m_registry;
    SDL_Renderer*   m_renderer;
};

#endif // INC_2D_CPP_ECS_GAME_RENDERSYSTEM_HPP
