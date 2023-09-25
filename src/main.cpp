#include <SDL2/SDL.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include "GameScene.hpp"


int main() {
    spdlog::cfg::load_env_levels();

    static constexpr int SCREEN_SIZE_X = 800;
    static constexpr int SCREEN_SIZE_Y = 600;
    static constexpr Uint32 FLAGS =
            SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_ALLOW_HIGHDPI;

    try {

        GameScene game_scene{"unnamed 2d cpp ecs game", SCREEN_SIZE_X, SCREEN_SIZE_Y, FLAGS};
        game_scene.Run();
    }
    catch (std::exception& exception) {
        spdlog::error("Application crashed with Exception: {}", exception.what());
        return EXIT_FAILURE;
    }
    catch (...) {
        spdlog::error("Application crashed with unknown exception!");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
