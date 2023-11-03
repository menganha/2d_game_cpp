#pragma once

#include "AssetManager.hpp"
#include "GameScene.hpp"
#include "Window.hpp"

class Game
{
  public:
    Game(std::string_view root_path);
    ~Game();
    void Run();

  private:
    Window       m_window;
    AssetManager m_asset_manager;
    GameScene    m_game_scene;
};
