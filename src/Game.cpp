#include "Game.hpp"

#include "Config.hpp"
#include "SDL2/SDL.h"

#include <filesystem>

Game::Game(std::string_view root_path_str) : m_window{}, m_asset_manager{}, m_game_scene{}
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
    std::filesystem::path root_path{ root_path_str };
    m_asset_manager = AssetManager((root_path.parent_path() / "assets").c_str());

    // Load resources
    m_asset_manager.AddFont("fonts/Anonymous Pro.ttf", 12, m_window.GetRenderer());
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
        m_window.ProcessEvents();
        m_game_scene.Run(m_asset_manager, m_window.GetRenderer());
    }
}
