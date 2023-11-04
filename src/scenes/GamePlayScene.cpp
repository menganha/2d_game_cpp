#include "GamePlayScene.hpp"

#include "../Colors.hpp"
#include "../Components.hpp"
#include "../Config.hpp"
#include "../Enemy.hpp"
#include "../Events.hpp"

#include <SDL2/SDL_ttf.h>
#include <spdlog/spdlog.h>

// AssetManager asset_manager)
//  , m_asset_manager{asset_manager}
GamePlayScene::GamePlayScene() 
  : m_gamepad{}
  , m_player_entity{}
  , m_registry{}
  , m_dispatcher{}
  , m_movement_system{ m_registry, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y }
  , m_collision_system{ m_registry, m_dispatcher, 
      Grid{ 0, 0, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y, Config::COLLISION_GRID_CELL_WIDTH, Config::COLLISION_GRID_CELL_HEIGHT } }
  , m_combat_system{ m_registry, m_dispatcher }
  , m_render_system{ m_registry }
  , m_enemy_system{ m_registry, m_dispatcher}
{

    // Sets some event listeners
    m_dispatcher.sink<SetEntityPositionEvent>().connect<&MovementSystem::OnSetEntityPositionEvent>(m_movement_system);
    m_dispatcher.sink<OutOfBoundariesEvent>().connect<&MovementSystem::OnOutOfBoundariesEvent>(m_movement_system);
    m_dispatcher.sink<ShootEvent>().connect<&CombatSystem::OnShootButtonEvent>(m_combat_system);
    m_dispatcher.sink<CollisionEvent>().connect<&CombatSystem::OnCollisionEvent>(m_combat_system);
    m_dispatcher.sink<OutOfBoundariesEvent>().connect<&CombatSystem::OnOutOfBoundariesEvent>(m_combat_system);
    m_dispatcher.sink<DeathEvent>().connect<&GamePlayScene::OnDeathEvent>(this);

    // Loads first level
    LoadLevel();
}

GamePlayScene::~GamePlayScene()
{
    m_dispatcher.clear();
}

void
GamePlayScene::ProcessEvents()
{

    // Keyboard processing
    auto keyboard_state = SDL_GetKeyboardState(nullptr);
    m_gamepad.Update(keyboard_state);

    // TODO: This should be the a constant somwhere (i.e., the velocity of the player entitty)
    if (m_gamepad.IsButtonDown(Gamepad::UP))
        m_movement_system.MoveEntity(Position{ 0.0f, -2.0f }, m_player_entity);
    if (m_gamepad.IsButtonDown(Gamepad::DOWN))
        m_movement_system.MoveEntity(Position{ 0.0f, 2.0f }, m_player_entity);
    if (m_gamepad.IsButtonDown(Gamepad::LEFT))
        m_movement_system.MoveEntity(Position{ -2.0f, 0.0f }, m_player_entity);
    if (m_gamepad.IsButtonDown(Gamepad::RIGHT))
        m_movement_system.MoveEntity(Position{ 2.0f, 0.0f }, m_player_entity);

    if (m_gamepad.IsButtonPressed(Gamepad::A))
        m_dispatcher.trigger(ShootEvent{ m_player_entity });

    // Dispatch any game other event accumulated from the previous frame here
    m_dispatcher.update();
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
    // Creates a "player" entity
    m_player_entity = m_registry.create();
    m_registry.emplace<Position>(m_player_entity, 402.f, 500.f);
    m_registry.emplace<SquarePrimitive>(m_player_entity, 10, 10, Colors::RED);
    m_registry.emplace<Velocity>(m_player_entity, 0.f, 0.f);
    m_registry.emplace<Collider>(m_player_entity, 10, 10, 0, 0, true);
    m_registry.emplace<Health>(m_player_entity, 40);

    // Creates 10 batches of enemies
    for (int idx = 1; idx <= 10; ++idx)
    {
        auto entity = m_registry.create();
        m_registry.emplace<EnemyHorde>(entity, EnemyType::PARAB, 40, 5, (idx - 1) * 500);
    }

    // Create UI elements
    auto entity = m_registry.create();
    m_registry.emplace<Position>(entity, 20.f, 20.f);
    m_registry.emplace<Text>(entity, "Lives: " + std::to_string(40), "fonts/Anonymous Pro.ttf", Colors::BLACK);
}

void
GamePlayScene::RestartLevel()
{
    m_registry.clear();
    LoadLevel();
}

void
GamePlayScene::OnDeathEvent(DeathEvent death_event)
{
    if (death_event.entity == m_player_entity)
    {
        RestartLevel();
    }
}
