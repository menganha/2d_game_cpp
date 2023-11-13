#pragma once

#include "AssetManager.hpp"
#include "Gamepad.hpp"
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
    Window                              m_window;
    AssetManager                        m_asset_manager;
    Gamepad                             m_gamepad;
    std::stack<std::shared_ptr<IScene>> m_scene_stack;
};
