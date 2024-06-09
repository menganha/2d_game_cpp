#include "PauseScene.hpp"

#include "../Colors.hpp"

#include <spdlog/spdlog.h>

PauseScene::PauseScene(AssetManager& asset_manager)
  : m_asset_manager{asset_manager}
  , m_imgui{}
{
}

void
PauseScene::ProcessEvents(const Gamepad& gamepad, [[maybe_unused]] SceneManager& scene_manager)
{
  m_imgui.UpdateGUIState(gamepad);

  if (m_imgui.Button("Continue Game", 20, 50)) {
    scene_manager.PopScene();
  }
  if (m_imgui.Button("Exit", 20, 100)) {
    scene_manager.PopScene();
    scene_manager.PopScene();
  }
}

void
PauseScene::Update([[maybe_unused]] uint64_t ticks)
{
}

void
PauseScene::Render(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);

  SDL_RenderClear(renderer);

  m_imgui.Render(m_asset_manager.GetFont("f21"), renderer);

  SDL_RenderPresent(renderer);
}
