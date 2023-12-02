#include "GamePlayScene.hpp"

#include "../Colors.hpp"
#include "../Config.hpp"
#include "../Events.hpp"

#include <SDL2/SDL_ttf.h>
#include <spdlog/spdlog.h>

GamePlayScene::GamePlayScene(const AssetManager &asset_manager)
    : m_registry{}, m_dispatcher{}, m_player{m_registry}
    , m_asset_manager{asset_manager}
    , m_movement_system{m_registry, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y}
    , m_collision_system{m_registry, m_dispatcher, 
        Grid{0, 0, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y, 
            Config::COLLISION_GRID_CELL_WIDTH, Config::COLLISION_GRID_CELL_HEIGHT}}
    , m_combat_system{m_registry, m_dispatcher}
    , m_render_system{m_registry}
    , m_enemy_system{m_registry, m_dispatcher}
    , m_cleanup_system{m_registry}
    , m_level_loader_system{m_registry, m_dispatcher}
    , m_hud{m_registry}
    , m_restart_level{false}
{
    // Sets some event listeners
    m_dispatcher.sink<SetEntityPositionEvent>().connect<&MovementSystem::OnSetEntityPositionEvent>(m_movement_system);
    m_dispatcher.sink<OutOfBoundariesEvent>().connect<&MovementSystem::OnOutOfBoundariesEvent>(m_movement_system);
    m_dispatcher.sink<BulletEvent>().connect<&CombatSystem::OnShootEvent>(m_combat_system);
    m_dispatcher.sink<CollisionEvent>().connect<&CombatSystem::OnCollisionEvent>(m_combat_system);
    m_dispatcher.sink<OutOfBoundariesEvent>().connect<&CombatSystem::OnOutOfBoundariesEvent>(m_combat_system);
    m_dispatcher.sink<HealthEvent>().connect<&HUD::OnHealthEvent>(m_hud);
    m_dispatcher.sink<DestroyEvent>().connect<&CleanUpSystem::OnDestroyEvent>(m_cleanup_system);
    m_dispatcher.sink<DestroyEvent>().connect<&GamePlayScene::OnDestroyEvent>(this);
    // Loads first level
    LoadLevel();
}

GamePlayScene::~GamePlayScene()
{
    m_dispatcher.clear();
}

void
GamePlayScene::ProcessEvents(const Gamepad& gamepad)
{
    // Dispatch any game other event accumulated from the previous frame here
    m_dispatcher.update();

    if (m_restart_level)
    {
        RestartLevel();
        m_restart_level = false;
    }

    // Input processing
    if (gamepad.IsButtonDown(Gamepad::UP))
        m_movement_system.MoveEntity(Position{0.0f, -m_player.kPlayerVelocity}, m_player.GetEntity());
    if (gamepad.IsButtonDown(Gamepad::DOWN))
        m_movement_system.MoveEntity(Position{0.0f, m_player.kPlayerVelocity}, m_player.GetEntity());
    if (gamepad.IsButtonDown(Gamepad::LEFT))
        m_movement_system.MoveEntity(Position{-m_player.kPlayerVelocity, 0.0f}, m_player.GetEntity());
    if (gamepad.IsButtonDown(Gamepad::RIGHT))
        m_movement_system.MoveEntity(Position{m_player.kPlayerVelocity, 0.0f}, m_player.GetEntity());
    if (gamepad.IsButtonPressed(Gamepad::A))
        m_player.Shoot();
    if (gamepad.IsButtonPressed(Gamepad::START))
        RequestChangeScene(SceneType::PAUSE_SCENE);
}

void
GamePlayScene::Update()
{
    m_movement_system.Update();
    m_collision_system.Update();
    m_dispatcher.update<CollisionEvent>(); // Process all collision events after pick them from the collision system
    m_dispatcher.update<OutOfBoundariesEvent>();
    m_level_loader_system.Update();
    m_enemy_system.Update(m_player.GetEntity());
    m_combat_system.Update();
}

void
GamePlayScene::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);

    SDL_RenderClear(renderer);

    m_render_system.Update(m_asset_manager, renderer);

    SDL_RenderPresent(renderer);
}

void
GamePlayScene::LoadLevel()
{
    // player
    m_player.Create();
    // HUD
    m_hud.Create(m_player.GetEntity());
    m_hud.Refresh(m_player.kPlayerInitialHealth);
    // Level Loader
    m_level_loader_system.LoadLevel(m_asset_manager.GetAbsolutePathStr("data/level_1"));
}

void
GamePlayScene::RestartLevel()
{
    m_registry.clear();

    LoadLevel();
}

void
GamePlayScene::OnDestroyEvent(DestroyEvent destroy_event)
{
    if (destroy_event.entity == m_player.GetEntity())
    {
        m_restart_level = true;
    }
}
