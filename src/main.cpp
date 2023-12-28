#include "Game.hpp"

#include <spdlog/cfg/env.h>
#include <spdlog/spdlog.h>
#include <stdlib.h>

int
main([[maybe_unused]] int argc, char* argv[])
{
    try
    {
        Game game{argv[0]};
        game.Run();
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
