#pragma once

#include "AssetManager.hpp"
#include "Gamepad.hpp"
#include "Window.hpp"
#include "scenes/IScene.hpp"

#include <filesystem>
#include <stack>

class Game
{
public:
    Game(std::string_view root_path);
    ~Game();
    void Run();

private:
    std::filesystem::path               m_root_path;
    Window                              m_window;
    AssetManager                        m_asset_manager;
    Gamepad                             m_gamepad;
    std::stack<std::shared_ptr<IScene>> m_scene_stack;
};
