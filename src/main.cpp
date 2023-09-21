#include <iostream>
#include <SDL2/SDL.h>
#include "GameScene.hpp"
#include <spdlog/cfg/env.h>



int main() {
    spdlog::cfg::load_env_levels();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::string err_msg = "SDL could not initialize! SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    SDL_Window *window_ = SDL_CreateWindow("Yet Another Tetris Clone",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           800,
                                           600,
                                           SDL_WINDOW_SHOWN);
    if (window_ == nullptr) {
        std::string err_msg = "Window could not be created! SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    SDL_Renderer *renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer_ == nullptr) {
        std::string err_msg = "Renderer could not be created SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    GameScene game_scene{renderer_};
    game_scene.Run();

    return 0;
}
