#include "Game.hpp"

#include "Config.hpp"
#include "SDL2/SDL.h"
#include "scenes/GamePlayScene.hpp"
#include "scenes/PauseScene.hpp"

#include "filesystem"

Game::Game(std::string_view root_path_str) : m_window{}, m_asset_manager{}, m_gamepad{}, m_scene_stack{}
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::string err_msg = "SDL2 could not initialize! SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }
    if (TTF_Init() < 0)
    {
        std::string err_msg = "SDL2_ttf could not initialize! SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    m_window = Window(Config::WINDOW_NAME, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y, Config::FLAGS);

    // Asset manager
    std::filesystem::path root_path{root_path_str};
    m_asset_manager = AssetManager(root_path.parent_path().c_str());

    // Load resources
    m_asset_manager.AddFont("fonts/PressStart2P.ttf", 20, m_window.GetRenderer());

    // Start the stack of scenes. Make it a shared ptr as not to worry about freeing it later
    m_scene_stack.push(std::make_shared<GamePlayScene>(m_asset_manager));
}

Game::~Game()
{
    TTF_Quit();
    SDL_Quit();
}

void
Game::Run()
{
    while (m_window.IsOpen())
    {
        if (m_scene_stack.empty())
        {
            break;
        }
        m_window.ProcessEvents();
        m_gamepad.Update(SDL_GetKeyboardState(nullptr));

        auto current_scene = m_scene_stack.top();
        current_scene->ProcessEvents(m_gamepad);
        current_scene->Update();
        current_scene->Render(m_window.GetRenderer());
        if (current_scene->HasEnded())
        {
            m_scene_stack.pop();
        }
        else if (current_scene->HasRequestedChange())
        {
            if (current_scene->GetNextScene() == SceneType::EXIT)
            {
                m_scene_stack = {};
            }
            else
            {
                current_scene->ResetSceneStatus();
                m_scene_stack.push(std::make_shared<PauseScene>(m_asset_manager));
            }
        }
    }
}
