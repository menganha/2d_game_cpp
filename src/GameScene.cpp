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
  , m_movement_system{ m_registry, entt::null, screen_width, screen_height }
  , m_collision_system{ m_registry, m_dispatcher, Grid{ 0, 0, screen_width, screen_height, 40, 40 } }
  , m_combat_system{ m_registry }
  , m_render_system{ m_registry }
{

    // Initializes SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::string err_msg = "SDL could not initialize! SDL Error: ";
        throw std::runtime_error(err_msg + SDL_GetError());
    }

    // Creates window
    m_window.Create(title, screen_width, screen_height, flags);

    // creates a "player" entity
    m_player_entity = m_registry.create();
    m_registry.emplace<Position>(m_player_entity, 402.f, 500.f);
    m_registry.emplace<Renderable>(m_player_entity, 10, 10, RED);
    m_registry.emplace<Velocity>(m_player_entity, 0.f, 0.f);
    m_registry.emplace<Movable>(m_player_entity);
    m_registry.emplace<Collider>(m_player_entity, 10, 10, 0, 0);
    spdlog::debug("Created entity with id {} at x={}, y={}", static_cast<int>(m_player_entity), 20, 0);

    // Creates a batch of enemies
    for (int i = 1; i < 10; ++i) {
        const auto entity = m_registry.create();
        m_registry.emplace<Position>(entity, 0.f, 27.f * i);
        m_registry.emplace<Collider>(entity, 20, 20, 0, 0);
        m_registry.emplace<Velocity>(entity, 1.f, 0.f);
        m_registry.emplace<Renderable>(entity, 20, 20, BLUE);
        m_registry.emplace<Health>(entity, 20);
        m_registry.emplace<Enemy>(entity, 0);
    }

    // initializes the systems
    m_render_system.Init(m_window.GetRenderer()); // TODO: Elminate these weird inits
    m_movement_system.Init(m_player_entity);

    // Sets some event listeners
    m_dispatcher.sink<DirectionalButtonEvent>().connect<&MovementSystem::OnDirectionalButtonEvent>(m_movement_system);
    m_dispatcher.sink<OutOfBoundariesEvent>().connect<&MovementSystem::OnOutOfBoundariesEvent>(m_movement_system);
    m_dispatcher.sink<ShootEvent>().connect<&CombatSystem::OnShootButtonEvent>(m_combat_system);
    m_dispatcher.sink<CollisionEvent>().connect<&CombatSystem::OnCollisionEvent>(m_combat_system);
    m_dispatcher.sink<OutOfBoundariesEvent>().connect<&CombatSystem::OnOutOfBoundariesEvent>(m_combat_system);
}

void
GameScene::Run()
{
    while (m_window.IsOpen()) {
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

    if (m_gamepad.IsDirectionalButtonEvent()) {
        m_dispatcher.trigger(DirectionalButtonEvent{ m_gamepad.GetKeyState(Gamepad::UP),
                                                     m_gamepad.GetKeyState(Gamepad::DOWN),
                                                     m_gamepad.GetKeyState(Gamepad::LEFT),
                                                     m_gamepad.GetKeyState(Gamepad::RIGHT) });
    }

    if (m_gamepad.IsButtonPressed(Gamepad::A)) {
        m_dispatcher.trigger(ShootEvent{ m_player_entity });
    }

    // Dispatch any game event accumulated from the previous frame here
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
}

void
GameScene::Render()
{

    SDL_SetRenderDrawColor(m_window.GetRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(m_window.GetRenderer());

    m_render_system.Update();

    SDL_RenderPresent(m_window.GetRenderer());
}
