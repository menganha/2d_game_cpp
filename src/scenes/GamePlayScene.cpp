#include "GamePlayScene.hpp"

#include "../Colors.hpp"
#include "../Config.hpp"
#include "../Entities.hpp"
#include "../Events.hpp"
#include "PauseScene.hpp"
#include "engine/Log.hpp"

GamePlayScene::GamePlayScene(AssetManager& asset_manager, int level)
  : m_registry{}
  , m_dispatcher{}
  , m_main_entities{}
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
  , m_script_system{m_registry, m_dispatcher}
  , m_cleanup_system{m_registry, m_dispatcher}
  , m_hud{m_registry}
  , m_level_data{}
  , m_lua_context{}
  , m_level_ended{false}
  , m_current_level{level}
{
  // Sets some event listeners
  // TODO: Good desing here would be that the systems themselves connect the handler.
  // The handles should be within the system, presumably
  m_dispatcher.sink<SetEntityPositionEvent>().connect<&MovementSystem::OnSetEntityPositionEvent>(m_movement_system);
  m_dispatcher.sink<OutOfBoundariesEvent>().connect<&MovementSystem::OnOutOfBoundariesEvent>(m_movement_system);
  m_dispatcher.sink<CollisionEvent>().connect<&CombatSystem::OnCollisionEvent>(m_combat_system);
  m_dispatcher.sink<OutOfBoundariesEvent>().connect<&CombatSystem::OnOutOfBoundariesEvent>(m_combat_system);
  m_dispatcher.sink<DamageEvent>().connect<&HUD::OnHealthEvent>(m_hud);
  m_dispatcher.sink<DestroyEvent>().connect<&GamePlayScene::OnDestroy>(this);
  m_dispatcher.sink<EndLevelEvent>().connect<&GamePlayScene::OnEndLevel>(this);

  LoadLevel(); // Loads intial level
}

GamePlayScene::~GamePlayScene()
{
  m_dispatcher.clear();
  m_registry.clear();
  // TODO: If there's an error early it cannot unwind since the video and music do not exists in the asset_manager
  m_asset_manager.GetVideo(m_level_data.video).StopDecodeThread();
  m_asset_manager.GetMusic(m_level_data.music).Halt();
}

void
GamePlayScene::ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager)
{
  // Dispatch any game other event accumulated from the previous frame here
  m_dispatcher.update<CollisionEvent>();
  m_dispatcher.update<OutOfBoundariesEvent>();
  m_dispatcher.update(); // TODO: Check!!! Are all events processed event the ones nested?

  if (m_level_ended and not m_script_system.PendingScripts()) {
    m_registry.clear();
    m_asset_manager.GetVideo(m_level_data.video).StopDecodeThread();
    m_asset_manager.GetMusic(m_level_data.music).Halt();
    m_level_ended = false;
    LoadLevel();
  }

  // Input processing
  if (gamepad.IsButtonDown(Gamepad::UP))
    m_movement_system.MoveEntity(Position{0.0f, -entities::PLAYER_VELOCITY}, m_main_entities.player);
  if (gamepad.IsButtonDown(Gamepad::DOWN))
    m_movement_system.MoveEntity(Position{0.0f, entities::PLAYER_VELOCITY}, m_main_entities.player);
  if (gamepad.IsButtonDown(Gamepad::LEFT))
    m_movement_system.MoveEntity(Position{-entities::PLAYER_VELOCITY, 0.0f}, m_main_entities.player);
  if (gamepad.IsButtonDown(Gamepad::RIGHT))
    m_movement_system.MoveEntity(Position{entities::PLAYER_VELOCITY, 0.0f}, m_main_entities.player);
  if (gamepad.IsButtonPressed(Gamepad::A))
    entities::PlayerShoot(m_main_entities.player, m_registry);
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

  m_cleanup_system.Update();
  m_script_system.Update();
  m_movement_system.Update();
  m_collision_system.Update();
  /*
     There should be some intermediate process of event to correct for collision agains solid objectes here.
     In this way we don't have to wait for a frame to correction and prevent artifacts such as being able
     to penetrate walls, etc.
  */
  m_enemy_system.Update(m_main_entities.player);
  m_text_system.Update();
  m_combat_system.Update();
}

void
GamePlayScene::Render(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);

  SDL_RenderClear(renderer);

  m_render_system.Update(m_asset_manager, renderer, m_main_entities.camera);

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
GamePlayScene::LoadLevel()
{

  // Add Player and Camera
  m_main_entities.player = entities::CreatePlayer(m_registry);
  m_main_entities.camera = entities::CreateCamera(m_registry);
  

  // Add HUD
  m_hud.Create(m_main_entities.player);
  m_hud.Refresh(entities::PLAYER_INITIAL_HEALTH);

  // Load Level Data
  auto script_path = m_asset_manager.GetAbsolutePathStr(intToLevelScript(m_current_level));
  LINFO("Intialiting level: %s", script_path.c_str());
  m_lua_context.GetLevelData(script_path.c_str(), m_level_data, m_registry, m_dispatcher, m_main_entities.player);

  // Add enemises
  m_enemy_system.SetEnemyList(m_level_data.enemy_list_to_dispatch);

  // Start playing music
  m_asset_manager.AddMusic(m_level_data.music.c_str());
  m_asset_manager.GetMusic(m_level_data.music).Play();

  // Add Video entity and start playing it forever ...
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
  if (destroy_event.entity == m_main_entities.player)
    m_level_ended = true;
}

void
GamePlayScene::OnEndLevel([[maybe_unused]] EndLevelEvent end_level_event)
{
  m_current_level += 1;
  m_level_ended = true;
}
