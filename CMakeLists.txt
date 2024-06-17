cmake_minimum_required(VERSION 3.26)
project(2d_cpp_ecs_game)

# Switch toggling between a systems library (usually linux) and a vendored (included) library. The latter is still not supported!
option(MYGAME_VENDORED "Use vendored libraries" OFF)


# SDL Library ========================================
if (MYGAME_VENDORED)
    add_subdirectory(libs/sdl EXCLUDE_FROM_ALL)
else ()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if any of them cannnot be found
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2)
    find_package(SDL2_image REQUIRED CONFIG COMPONENTS SDL2_image)
    find_package(SDL2_mixer REQUIRED CONFIG COMPONENTS SDL2_mixer)
    find_package(SDL2_ttf REQUIRED CONFIG COMPONENTS SDL2_ttf)
    # 1. Look for a SDL2 package, 2. Look for the SDL2main component and 3. DO NOT fail when SDL2main is not available
    find_package(SDL2 CONFIG COMPONENTS SDL2main)
endif ()


# Lua Librarysection =====================================
if (MYGAME_VENDORED)
    add_subdirectory(libs/lua EXCLUDE_FROM_ALL)
else ()
  find_package(Lua REQUIRED)
endif ()


# FFMPEG section ======================================
# NOTE: We still do not have a solution for a vendored (included) ffmpeg library
find_library(AVCODEC_LIBRARY avcodec)
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVFORMAT_LIBRARY avformat)
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVUTIL_LIBRARY avutil)
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(SWSCALE_LIBRARY swscale)
find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)


# Main sources =================================================
add_executable(app 
        src/main.cpp
        src/Config.hpp
        src/Components.hpp
        src/Events.hpp
        src/Collision.cpp
        src/Collision.hpp
        src/HUD.cpp
        src/HUD.hpp
        src/Enemy.cpp
        src/Enemy.hpp
        src/Player.cpp
        src/Player.hpp
        src/scenes/IntroScene.cpp
        src/scenes/IntroScene.hpp
        src/scenes/GamePlayScene.cpp
        src/scenes/GamePlayScene.hpp
        src/scenes/PauseScene.cpp
        src/scenes/PauseScene.hpp
        src/systems/MovementSystem.cpp
        src/systems/MovementSystem.hpp
        src/systems/CollisionSystem.cpp
        src/systems/CollisionSystem.hpp
        src/systems/CleanUpSystem.cpp
        src/systems/CleanUpSystem.hpp
        src/systems/LevelLoaderSystem.cpp
        src/systems/LevelLoaderSystem.hpp
        src/systems/RenderSystem.cpp
        src/systems/RenderSystem.hpp
        src/systems/CombatSystem.cpp
        src/systems/CombatSystem.hpp
        src/systems/EnemySystem.cpp
        src/systems/EnemySystem.hpp
        src/engine/AssetManager.hpp
        src/engine/AssetManager.cpp
        src/engine/Log.hpp
        src/engine/Log.cpp
        src/engine/IScene.hpp
        src/engine/IScene.cpp
        src/engine/Gamepad.cpp
        src/engine/Gamepad.hpp
        src/engine/GameLoop.cpp
        src/engine/GameLoop.hpp
        src/engine/Font.cpp
        src/engine/Font.hpp
        src/engine/Video.cpp
        src/engine/Video.hpp
        src/engine/Texture.cpp
        src/engine/Texture.hpp
        src/engine/Window.cpp
        src/engine/Window.hpp
        src/engine/IMGUI.cpp
        src/engine/IMGUI.hpp
        )

# Always set the name to app for easily passing this variable to vim
set_target_properties(app PROPERTIES OUTPUT_NAME "app")
        

# Include or add directories
add_subdirectory(libs/entt)
add_subdirectory(libs/spdlog)
add_subdirectory(libs/sol2/single)
target_include_directories(app PUBLIC ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR})


# Compile features and options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
target_compile_definitions(EnTT INTERFACE ENTT_DISABLE_ASSERT)  # Improves compilation time by disabling the assert
target_compile_options(app PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>)
target_compile_features(app PRIVATE cxx_std_17)


# Link libraries
if (TARGET SDL2::SDL2main)
    # SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(app PRIVATE SDL2::SDL2main)
endif ()

target_link_libraries(app PRIVATE 
    SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf 
    EnTT::EnTT 
    spdlog::spdlog 
    sol2::sol2::single 
    ${LUA_LIBRARIES}
    ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${SWSCALE_LIBRARY}
)

# Custom commands. Create symlinks for bin directories. This can be latter added to a post build (install script)
add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:app>/assets 
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:app>/data 
    VERBATIM)

