#pragma once

#include "engine/AssetManager.hpp"

#include <SDL2/SDL.h>
#include <entt/fwd.hpp>

class RenderSystem
{

public:
  constexpr static int BLINKING_PERIOD = 8;
  RenderSystem(entt::registry& registry);
  void Update(AssetManager& asset_manager, SDL_Renderer* renderer, entt::entity);

private:
  entt::registry& m_registry;
  SDL_Renderer*   m_renderer;

// TEST
  float m_scaling = 1.0f;
};
