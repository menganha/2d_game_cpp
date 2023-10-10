cmake_minimum_required(VERSION 3.26)
project(2d_cpp_ecs_game)

# Create an option to switch between a system sdl library and a vendored sdl library
option(MYGAME_VENDORED "Use vendored libraries" OFF)

if (MYGAME_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else ()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2)
    find_package(SDL2_image REQUIRED CONFIG COMPONENTS SDL2_image)
    find_package(SDL2_mixer REQUIRED CONFIG COMPONENTS SDL2_mixer)
    find_package(SDL2_ttf REQUIRED CONFIG COMPONENTS SDL2_ttf)
    # 1. Look for a SDL2 package, 2. Look for the SDL2main component and 3. DO NOT fail when SDL2main is not available
    find_package(SDL2 CONFIG COMPONENTS SDL2main)
endif ()

add_executable(
        2d_cpp_ecs_game src/main.cpp
        src/GameScene.cpp
        src/GameScene.hpp
        src/Gamepad.cpp
        src/Gamepad.hpp
        src/Components.hpp
        src/systems/MovementSystem.cpp
        src/systems/MovementSystem.hpp
        src/Events.hpp
        src/systems/CollisionSystem.cpp
        src/systems/CollisionSystem.hpp
        src/systems/RenderSystem.cpp
        src/systems/RenderSystem.hpp
        src/Window.cpp
        src/Window.hpp
        src/Collision.cpp
        src/Collision.hpp
        src/systems/CombatSystem.cpp
        src/systems/CombatSystem.hpp
        src/systems/EnemySystem.cpp
        src/systems/EnemySystem.hpp
        )
        

# Include or add directories
#target_include_directories(2d_cpp_ecs_game PRIVATE libs)
add_subdirectory(libs/entt)
target_compile_definitions(EnTT INTERFACE ENTT_DISABLE_ASSERT)  # Improves compilation time by disabling the assert

# compile features and options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # To generate compile commands for vim YCM. Set it on perhaps only on 
add_subdirectory(libs/spdlog)
target_compile_features(2d_cpp_ecs_game PRIVATE cxx_std_17)
target_compile_options(2d_cpp_ecs_game PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
# Link libraries
if (TARGET SDL2::SDL2main)
    # SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(2d_cpp_ecs_game PRIVATE SDL2::SDL2main)
endif ()

target_link_libraries(2d_cpp_ecs_game PRIVATE SDL2::SDL2 SDL2_image::SDL2_image EnTT::EnTT spdlog::spdlog)
