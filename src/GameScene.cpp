#include "GameScene.hpp"

#include "Components.hpp"
#include "Events.hpp"

#include <spdlog/spdlog.h>

GameScene::GameScene(const char* title, int screen_width, int screen_height, Uint32 flags)
  : m_gamepad{}
  , m_window{}
  , m_player_entity{}
  , m_registry{}
  , m_dispatcher{}
  , m_movement_system{ m_registry, screen_width, screen_height }
  , m_collision_system{ m_registry, m_dispatcher, Grid{ 0, 0, screen_width, screen_height, 40, 40 } }
  , m_combat_system{ m_registry }
  , m_render_system{ m_registry }
  , m_enemy_system{ m_registry, m_dispatcher}
{

    // Initializes SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::string err_msg = "SDL could not initialize! SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    // Creates window
    m_window.Create(title, screen_width, screen_height, flags);

    // Creates a "player" entity
    m_player_entity = m_registry.create();
    m_registry.emplace<Position>(m_player_entity, 402.f, 500.f);
    m_registry.emplace<Renderable>(m_player_entity, 10, 10, RED);
    m_registry.emplace<Velocity>(m_player_entity, 0.f, 0.f);
    m_registry.emplace<Movable>(m_player_entity);
    m_registry.emplace<Collider>(m_player_entity, 10, 10, 0, 0, true);
    spdlog::debug("Created entity with id {} at x={}, y={}", static_cast<int>(m_player_entity), 20, 0);

    // Creates a batch of enemies
    // const auto entity = m_registry.create();
    // m_registry.emplace<EnemyHorde>(entity, 0, 4, 1000);

    // initializes the systems
    m_render_system.Init(m_window.GetRenderer()); // TODO: Elminate these weird inits

    // Sets some event listeners
    // m_dispatcher.sink<EEnemyMovement>().connect<&MovementSystem::OnOutOfBoundariesEvent>(m_movement_system);
    m_dispatcher.sink<SetEntityPositionEvent>().connect<&MovementSystem::OnSetEntityPositionEvent>(m_movement_system);
    m_dispatcher.sink<OutOfBoundariesEvent>().connect<&MovementSystem::OnOutOfBoundariesEvent>(m_movement_system);
    m_dispatcher.sink<ShootEvent>().connect<&CombatSystem::OnShootButtonEvent>(m_combat_system);
    m_dispatcher.sink<CollisionEvent>().connect<&CombatSystem::OnCollisionEvent>(m_combat_system);
    m_dispatcher.sink<OutOfBoundariesEvent>().connect<&CombatSystem::OnOutOfBoundariesEvent>(m_combat_system);
}

void
GameScene::Run()
{
    while (m_window.IsOpen())
    {
        ProcessEvents();
        UpdateLogic();
        Render();
    }
}

void
GameScene::ProcessEvents()
{

    // Process events from the SDL window
    m_window.ProcessEvents();

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
GameScene::UpdateLogic()
{
    m_movement_system.Update();
    m_collision_system.Update();
    m_dispatcher.update<CollisionEvent>(); // Process all collision events once we pick them up
    m_dispatcher.update<OutOfBoundariesEvent>();
    m_combat_system.Update();
    m_enemy_system.Update();
}

void
GameScene::Render()
{

    SDL_SetRenderDrawColor(m_window.GetRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(m_window.GetRenderer());

    m_render_system.Update();

    SDL_RenderPresent(m_window.GetRenderer());
}
