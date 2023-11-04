#pragma once

#include "AssetManager.hpp"
#include "Window.hpp"
#include "scenes/IScene.hpp"
#include <stack>

class Game
{
  public:
    Game(std::string_view root_path);
    ~Game();
    void Run();

  private:
    Window       m_window;
    AssetManager m_asset_manager;
    std::stack<std::shared_ptr<IScene>> m_scene_stack;
};
