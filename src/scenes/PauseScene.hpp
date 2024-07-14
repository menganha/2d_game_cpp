#pragma once

#include "engine/Gamepad.hpp"
#include "engine/IMGUI.hpp"
#include "engine/IScene.hpp"
#include "engine/AssetManager.hpp"

#include <SDL2/SDL.h>
#include <entt/fwd.hpp>

class PauseScene : public IScene
{
public:
  PauseScene(AssetManager& asset_manager);
  void ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager) override;
  void Update(uint64_t ticks) override;
  void Render(SDL_Renderer* renderer) override;

private:
  void          StupidFunction();
  AssetManager& m_asset_manager;
  IMGUI         m_imgui;
};
