#pragma once

#include "engine/AssetManager.hpp"
#include "engine/Font.hpp"
#include "engine/Gamepad.hpp"
#include "engine/IMGUI.hpp"
#include "engine/IScene.hpp"

#include <SDL2/SDL.h>

class IntroScene : public IScene
{
public:
  IntroScene(AssetManager& asset_manager, int init_level = 0);
  void ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager) override;
  void Update(uint64_t ticks) override;
  void Render(SDL_Renderer* renderer) override;

private:
  AssetManager& m_asset_manager;
  int           m_level;
  Font          m_font;
  IMGUI         m_imgui;
  const char*   m_game_title = "YASTUP";
  const char*   m_start_game_text = "Start Game";
  const char*   m_exit_text = "Exit";
};
