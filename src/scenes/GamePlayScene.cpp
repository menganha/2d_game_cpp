#include "GamePlayScene.hpp"

#include "../Colors.hpp"
#include "../Components.hpp"
#include "../Config.hpp"
#include "../Enemy.hpp"
#include "../Events.hpp"

#include <SDL2/SDL_ttf.h>
#include <spdlog/spdlog.h>

GamePlayScene::GamePlayScene() 
  : m_player_entity{}
  , m_registry{}
  , m_dispatcher{}
  , m_movement_system{ m_registry, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y }
  , m_collision_system{ 
      m_registry, 
      m_dispatcher, 
      Grid{ 0, 0, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y, Config::COLLISION_GRID_CELL_WIDTH, Config::COLLISION_GRID_CELL_HEIGHT } }
  , m_combat_system{ m_registry, m_dispatcher }
  , m_render_system{ m_registry }
  , m_enemy_system{ m_registry, m_dispatcher}
  , m_cleanup_system{m_registry}
  , m_hud{m_registry}
  , m_restart_level{false}
{
    // Sets some event listeners
    m_dispatcher.sink<SetEntityPositionEvent>().connect<&MovementSystem::OnSetEntityPositionEvent>(m_movement_system);
    m_dispatcher.sink<OutOfBoundariesEvent>().connect<&MovementSystem::OnOutOfBoundariesEvent>(m_movement_system);
    m_dispatcher.sink<ShootEvent>().connect<&CombatSystem::OnShootEvent>(m_combat_system);
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
        m_movement_system.MoveEntity(Position{ 0.0f, -Config::kPlayerVelocity }, m_player_entity);
    if (gamepad.IsButtonDown(Gamepad::DOWN))
        m_movement_system.MoveEntity(Position{ 0.0f, Config::kPlayerVelocity }, m_player_entity);
    if (gamepad.IsButtonDown(Gamepad::LEFT))
        m_movement_system.MoveEntity(Position{ -Config::kPlayerVelocity, 0.0f }, m_player_entity);
    if (gamepad.IsButtonDown(Gamepad::RIGHT))
        m_movement_system.MoveEntity(Position{ Config::kPlayerVelocity, 0.0f }, m_player_entity);
    if (gamepad.IsButtonPressed(Gamepad::A))
        m_combat_system.OnShootEvent(ShootEvent{ m_player_entity });
    if (gamepad.IsButtonPressed(Gamepad::START))
        RequestChangeScene(SceneType::PAUSE_SCENE);
}

void
GamePlayScene::Update()
{
    m_movement_system.Update();
    m_collision_system.Update();
    m_dispatcher.update<CollisionEvent>(); // Process all collision events once we pick them up
    m_dispatcher.update<OutOfBoundariesEvent>();
    m_combat_system.Update();
    m_enemy_system.Update();
}

void
GamePlayScene::Render(const AssetManager& asset_manager, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);

    SDL_RenderClear(renderer);

    m_render_system.Update(asset_manager, renderer);

    SDL_RenderPresent(renderer);
}

void
GamePlayScene::LoadLevel()
{
    // TODO: Creates a "player" entity (Maybe move this to it's own namespace/file/class)
    m_player_entity = m_registry.create();
    spdlog::info("Creating player entity with id {}", static_cast<int>(m_player_entity));
    m_registry.emplace<Position>(m_player_entity, 402.f, 500.f);
    m_registry.emplace<SquarePrimitive>(m_player_entity, 10, 10, Colors::RED);
    m_registry.emplace<Velocity>(m_player_entity, 0.f, 0.f);
    m_registry.emplace<Collider>(m_player_entity, 10, 10, 0, 0, true);
    m_registry.emplace<Health>(m_player_entity, Config::kPlayerInitialHealth);

    // HUD
    m_hud.Init(Config::kPlayerInitialHealth, m_player_entity);

    // Creates batches of enemies
    EnemyList enemy_list;
    for (int idx = 1; idx <= 3; ++idx)
    {
        enemy_list.push_back(EnemyEntry{ EnemyType::SIMPLE, idx * 10, idx * 40, 0 });
    }
   
    for (int idx = 1; idx <= 3; ++idx)
    {
        enemy_list.push_back(EnemyEntry{ EnemyType::SIMPLE, (idx * 20) + 60, (idx * 40) + 200, 0 });
    }

    for (int idx = 1; idx <= 3; ++idx)
    {
        enemy_list.push_back(EnemyEntry{ EnemyType::SIMPLE, (idx * 20) + 120, (idx * 40) + 600, 0 });
    }

    m_enemy_system.SetEnemyList(std::move(enemy_list));
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
    if (destroy_event.entity == m_player_entity)
    {
        m_restart_level = true;
    }
}
