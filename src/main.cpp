#include "engine/AssetManager.hpp"
#include "engine/GameLoop.hpp"
#include "engine/IScene.hpp"
#include "engine/Gamepad.hpp"
// #include "scenes/GamePlayScene.hpp"
#include "scenes/IntroScene.hpp"
#include "Config.hpp"

#include <SDL2/SDL.h>
#include <spdlog/cfg/env.h>
#include <spdlog/spdlog.h>

int
main([[maybe_unused]] int argc, char* argv[])
{
    try
    {
        spdlog::cfg::load_env_levels(); // Checks the env variables to set the LOG levels. TODO: Take it away and replace
                                        // calls to log function by it macros so that we can disable them in the game logs
        // Initialize SDL Video and TTF
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::string err_msg = "SDL2 could not initialize! SDL Error: ";
            spdlog::error(err_msg + SDL_GetError());
            return EXIT_FAILURE;
        }
        if (TTF_Init() < 0)
        {
            std::string err_msg = "SDL2_ttf could not initialize! SDL Error: ";
            spdlog::error(err_msg + SDL_GetError());
            return EXIT_FAILURE;
        }

        // Asset manager and scene manager. 
        // It is important to instantiate the window before the asset manager so that the renderer destructor is called last when it goes out of scope.
        // If called earlier then all all textured are free'd and then textures on the asset manager are tried free'd again when we destroy the object
        Window       window{Config::WINDOW_NAME, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y, Config::FLAGS};
        AssetManager asset_manager{argv[0]};
        SceneManager scene_manager{};
        Gamepad gamepad{};


        // Load data
        asset_manager.AddFont("fonts/PressStart2P.ttf", 21, window.GetRenderer(), "f21");
        asset_manager.AddFont("fonts/PressStart2P.ttf", 30, window.GetRenderer(), "f30");
        asset_manager.AddVideo("videos/UFO.mp4", window.GetRenderer());
        asset_manager.AddVideo("videos/pokemon.mp4", window.GetRenderer());

        std::shared_ptr<IScene> game_play_scene{std::make_shared<IntroScene>(asset_manager)};
        scene_manager.PushScene(game_play_scene);
        RunLoop(gamepad, window, scene_manager);

        TTF_Quit();
        SDL_Quit();
    }
    catch (std::exception& exception)
    {
        spdlog::error("Application crashed with Exception: {}", exception.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        spdlog::error("Application crashed with unknown exception!");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
