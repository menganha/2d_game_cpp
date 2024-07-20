cmake_minimum_required(VERSION 3.29)
project(2d_cpp_ecs_game)

# Main sources 
add_executable(app 
        src/main.cpp
        src/Collision.cpp
        src/HUD.cpp
        src/Enemy.cpp
        src/Player.cpp
        src/LuaState.cpp
        src/LuaContext.cpp
        src/scenes/IntroScene.cpp
        src/scenes/GamePlayScene.cpp
        src/scenes/PauseScene.cpp
        src/systems/MovementSystem.cpp
        src/systems/CollisionSystem.cpp
        src/systems/CleanUpSystem.cpp
        src/systems/RenderSystem.cpp
        src/systems/CombatSystem.cpp
        src/systems/EnemySystem.cpp
        src/systems/TextSystem.cpp
    )

# Always set the name to app for easily passing this variable to vim
set_target_properties(app PROPERTIES OUTPUT_NAME "app")

# Lua, Entt, and the engine
find_package(Lua REQUIRED)
add_subdirectory(libs/entt)
add_subdirectory(libs/yagm)

# target_compile_definitions(EnTT INTERFACE ENTT_DISABLE_ASSERT)  # Improves compilation time by disabling the assert
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Werror=format)
target_compile_features(app PRIVATE cxx_std_17)

# includes and link libraries
target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(app PRIVATE EnTT::EnTT engine ${LUA_LIBRARIES})

# Custom commands. Create symlinks for bin directories. This can be latter added to a post build (install script)
add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:app>/assets 
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:app>/data 
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/scripts $<TARGET_FILE_DIR:app>/scripts
    VERBATIM)

