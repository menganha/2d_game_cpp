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

add_executable(app 
        src/main.cpp
        src/Config.hpp
        src/AssetManager.hpp
        src/AssetManager.cpp
        src/Gamepad.cpp
        src/Gamepad.hpp
        src/Game.cpp
        src/Game.hpp
        src/Enemy.cpp
        src/Enemy.hpp
        src/Font.cpp
        src/Font.hpp
        src/Components.hpp
        src/Events.hpp
        src/Window.cpp
        src/Window.hpp
        src/Collision.cpp
        src/Collision.hpp
        src/scenes/GamePlayScene.cpp
        src/scenes/GamePlayScene.hpp
        src/scenes/PauseScene.cpp
        src/scenes/PauseScene.hpp
        src/scenes/IScene.hpp
        src/systems/MovementSystem.cpp
        src/systems/MovementSystem.hpp
        src/systems/CollisionSystem.cpp
        src/systems/CollisionSystem.hpp
        src/systems/CleanUpSystem.cpp
        src/systems/CleanUpSystem.hpp
        src/systems/LevelLoaderSystem.cpp
        src/systems/LevelLoaderSystem.hpp
        src/HUD.cpp
        src/HUD.hpp
        src/Player.cpp
        src/Player.hpp
        src/systems/RenderSystem.cpp
        src/systems/RenderSystem.hpp
        src/systems/CombatSystem.cpp
        src/systems/CombatSystem.hpp
        src/systems/EnemySystem.cpp
        src/systems/EnemySystem.hpp
        src/widgets/Widget.hpp
        src/widgets/WidgetContainer.hpp
        src/widgets/WidgetContainer.cpp
        src/widgets/ButtonWidget.hpp
        src/widgets/ButtonWidget.cpp
        )

# Always set the name to app for easily passing this variable to vim
set_target_properties(app PROPERTIES OUTPUT_NAME "app")
        

# Include or add directories
add_subdirectory(libs/entt)
add_subdirectory(libs/spdlog)


# Compile features and options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
target_compile_definitions(EnTT INTERFACE ENTT_DISABLE_ASSERT)  # Improves compilation time by disabling the assert
target_compile_options(app PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
target_compile_features(app PRIVATE cxx_std_17)


# Link libraries
if (TARGET SDL2::SDL2main)
    # SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(app PRIVATE SDL2::SDL2main)
endif ()

target_link_libraries(app PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf EnTT::EnTT spdlog::spdlog)

# Custom commands. Create symlinks for bin directories
add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:app>/assets 
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:app>/data 
    VERBATIM)
