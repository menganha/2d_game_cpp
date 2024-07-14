#include "Config.hpp"
#include "engine/AssetManager.hpp"
#include "engine/GameLoop.hpp"
#include "engine/Gamepad.hpp"
#include "engine/IScene.hpp"
#include "engine/Log.hpp"
#include "scenes/IntroScene.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

int
main([[maybe_unused]] int argc, char* argv[])
{

  // Initialize SDL Video and TTF
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    LERROR("SDL2 could not initialize! SDL Error: %s", SDL_GetError());
    return EXIT_FAILURE;
  }
  if (TTF_Init() < 0) {
    LERROR("SDL2_ttf could not initialize! SDL Error: %s", SDL_GetError());
    return EXIT_FAILURE;
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    LERROR("SDL2_mixer could not initialize! SDL Error: %s", Mix_GetError());
    return EXIT_FAILURE;
  }

  try {
    // Asset manager and scene manager.
    // It is important to instantiate the window before the asset manager so that the renderer destructor is called last
    // when it goes out of scope. If called earlier then all all textured are free'd and then textures on the asset
    // manager are tried free'd again when we destroy the object
    Window       window{Config::WINDOW_NAME, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y, Config::FLAGS};
    AssetManager asset_manager{argv[0]};
    SceneManager scene_manager{};
    Gamepad      gamepad{};

    // Load data
    asset_manager.AddFont("fonts/PressStart2P.ttf", 21, window.GetRenderer(), "f21");
    asset_manager.AddFont("fonts/PressStart2P.ttf", 30, window.GetRenderer(), "f30");
    asset_manager.AddVideo("videos/clouds.mp4", window.GetRenderer());
    asset_manager.AddMusic("music/penso_positivo.wav");

    std::shared_ptr<IScene> game_play_scene{std::make_shared<IntroScene>(asset_manager)};
    scene_manager.PushScene(game_play_scene);
    RunLoop(gamepad, window, scene_manager);

    TTF_Quit();
    SDL_Quit();

  } catch (std::exception& exception) {
    LERROR("Application crashed with Exception: %s", exception.what());
    return EXIT_FAILURE;
  } catch (...) {
    LERROR("Application crashed with unknown exception!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
