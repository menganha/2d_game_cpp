#pragma once

#include "../AssetManager.hpp"

#include <SDL2/SDL.h>
#include <entt/fwd.hpp>

class RenderSystem
{

public:
    constexpr static int BLINKING_PERIOD = 8;
    RenderSystem(const entt::registry& registry);
    void Update(AssetManager& asset_manager, SDL_Renderer* renderer);

private:
    const entt::registry& m_registry;
    SDL_Renderer*         m_renderer;
};
