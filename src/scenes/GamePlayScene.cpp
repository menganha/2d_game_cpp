#include "GamePlayScene.hpp"

#include "../Colors.hpp"
#include "../Config.hpp"
#include "../Events.hpp"
#include "PauseScene.hpp"

GamePlayScene::GamePlayScene(AssetManager& asset_manager, std::string_view initial_level_path)
  : m_registry{}
  , m_dispatcher{}
  , m_player{m_registry}
  , m_asset_manager{asset_manager}
  , m_movement_system{m_registry, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y}
  , m_collision_system{
      m_registry, 
      m_dispatcher, 
      Grid{0, 0, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y, Config::COLLISION_GRID_CELL_WIDTH, Config::COLLISION_GRID_CELL_HEIGHT} }
  , m_combat_system{m_registry, m_dispatcher}
  , m_render_system{m_registry}
  , m_enemy_system{m_registry, m_dispatcher}
  , m_text_system{m_registry, m_asset_manager}
  , m_cleanup_system{m_registry, m_dispatcher}
  , m_hud{m_registry}
  , m_level_data{}
  , m_lua_context{}
  , m_restart_level{false}
  , m_level_finished{false}
{
  // Sets some event listeners
  // TODO: Good desing here would be that the systems themselves connect the handler. 
  // The handles should be within the system
  m_dispatcher.sink<SetEntityPositionEvent>().connect<&MovementSystem::OnSetEntityPositionEvent>(m_movement_system);
  m_dispatcher.sink<OutOfBoundariesEvent>().connect<&MovementSystem::OnOutOfBoundariesEvent>(m_movement_system);
  m_dispatcher.sink<CollisionEvent>().connect<&CombatSystem::OnCollisionEvent>(m_combat_system);
  m_dispatcher.sink<OutOfBoundariesEvent>().connect<&CombatSystem::OnOutOfBoundariesEvent>(m_combat_system);
  m_dispatcher.sink<DamageEvent>().connect<&HUD::OnHealthEvent>(m_hud);
  m_dispatcher.sink<DestroyEvent>().connect<&GamePlayScene::OnDestroy>(this);
  m_dispatcher.sink<EndLevelEvent>().connect<&GamePlayScene::OnEndLevel>(this);

  // Loads first level
  LoadLevel(initial_level_path.data());
}

GamePlayScene::~GamePlayScene()
{
  m_asset_manager.GetVideo(m_level_data.video).StopDecodeThread();
  m_dispatcher.clear();
}

void
GamePlayScene::ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager)
{
  // Dispatch any game other event accumulated from the previous frame here
  m_dispatcher.update(); // TODO: test!!! Are all events processed event the ones nested?

  if (m_restart_level or m_level_finished) { // TODO: Very similar! Refactor
    m_registry.clear();
    m_asset_manager.GetVideo(m_level_data.video).StopDecodeThread();
    m_restart_level = m_level_finished = false;
    LoadLevel();
  }

  // Input processing
  if (gamepad.IsButtonDown(Gamepad::UP))
    m_movement_system.MoveEntity(Position{0.0f, -m_player.PLAYER_VELOCITY}, m_player.GetEntity());
  if (gamepad.IsButtonDown(Gamepad::DOWN))
    m_movement_system.MoveEntity(Position{0.0f, m_player.PLAYER_VELOCITY}, m_player.GetEntity());
  if (gamepad.IsButtonDown(Gamepad::LEFT))
    m_movement_system.MoveEntity(Position{-m_player.PLAYER_VELOCITY, 0.0f}, m_player.GetEntity());
  if (gamepad.IsButtonDown(Gamepad::RIGHT))
    m_movement_system.MoveEntity(Position{m_player.PLAYER_VELOCITY, 0.0f}, m_player.GetEntity());
  if (gamepad.IsButtonPressed(Gamepad::A))
    m_player.Shoot();
  if (gamepad.IsButtonPressed(Gamepad::SELECT))
    scene_manager.PopScene();
  if (gamepad.IsButtonPressed(Gamepad::START)) {
    std::shared_ptr<IScene> pause_scene = std::make_shared<PauseScene>(m_asset_manager);
    scene_manager.PushScene(pause_scene);
  }
}

void
GamePlayScene::Update([[maybe_unused]] uint64_t ticks)
{
  m_asset_manager.GetVideo(m_level_data.video).UpdateTexture();
  m_movement_system.Update();
  m_collision_system.Update();

  m_dispatcher.update<CollisionEvent>(); // Process all collision events after pick them from the collision system
  m_dispatcher.update<OutOfBoundariesEvent>();

  m_enemy_system.Update(m_player.GetEntity());
  m_text_system.Update();
  m_combat_system.Update();
  m_cleanup_system.Update();
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
GamePlayScene::OnEnter()
{
  m_asset_manager.GetMusic(m_level_data.music).Resume();
}

void
GamePlayScene::OnExit()
{
  m_asset_manager.GetMusic(m_level_data.music).Pause();
}

void
GamePlayScene::LoadLevel(const char* level_config_path)
{

  if (level_config_path)
    m_level_data = m_lua_context.GetLevelData(level_config_path); // Sets the level data
  // Player
  m_player.Create();

  // HUD
  m_hud.Create(m_player.GetEntity());
  m_hud.Refresh(m_player.PLAYER_INITIAL_HEALTH);

  // Level Loader
  m_enemy_system.SetEnemyList(m_level_data.enemy_list_to_dispatch);

  // Start playing music
  m_asset_manager.AddMusic(m_level_data.music.c_str());
  m_asset_manager.GetMusic(m_level_data.music).Play();

  // Add Video entity and start playing it forever
  auto entity = m_registry.create();
  m_asset_manager.AddVideo(m_level_data.video.c_str());
  m_asset_manager.GetVideo(m_level_data.video).StartDecodeThread(-1); // Start playing it
  m_registry.emplace<Position>(entity, 0.f, 0.f, -1);
  m_registry.emplace<Renderable>(entity, m_level_data.video, Config::SCREEN_SIZE_X, Config::SCREEN_SIZE_Y);

  // Add text entity
  auto text_entity = m_registry.create();
  m_registry.emplace<Position>(text_entity, 0.f, 60.f);
  m_registry.emplace<TypedText>(text_entity, m_level_data.text, Config::font_s, Colors::BLUE);
}

void
GamePlayScene::OnDestroy(DestroyEvent destroy_event)
{
  if (destroy_event.entity == m_player.GetEntity()) {
    m_restart_level = true;
  }
}

void
GamePlayScene::OnEndLevel([[maybe_unused]] EndLevelEvent end_level_event)
{
  m_level_finished = true;
}
