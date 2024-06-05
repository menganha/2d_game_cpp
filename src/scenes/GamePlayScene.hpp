#pragma once

#include "../HUD.hpp"
#include "../Player.hpp"
#include "../engine/AssetManager.hpp"
#include "../engine/Gamepad.hpp"
#include "../engine/IScene.hpp"
#include "../engine/Video.hpp"
#include "../systems/CleanUpSystem.hpp"
#include "../systems/CollisionSystem.hpp"
#include "../systems/CombatSystem.hpp"
#include "../systems/EnemySystem.hpp"
#include "../systems/LevelLoaderSystem.hpp"
#include "../systems/MovementSystem.hpp"
#include "../systems/RenderSystem.hpp"

#include <SDL2/SDL.h>
#include <entt/fwd.hpp>

class GamePlayScene : public IScene
{
public:
  GamePlayScene(AssetManager& asset_manager);
  ~GamePlayScene();
  void ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager) override;
  void Update() override;
  void Render(SDL_Renderer* renderer) override;

private:
  void LoadLevel();
  void RestartLevel();
  void OnDestroyEvent(DestroyEvent destroy_event);

  entt::registry    m_registry;
  entt::dispatcher  m_dispatcher;
  Player            m_player;
  AssetManager&     m_asset_manager;
  MovementSystem    m_movement_system;
  CollisionSystem   m_collision_system;
  CombatSystem      m_combat_system;
  RenderSystem      m_render_system;
  EnemySystem       m_enemy_system;
  CleanUpSystem     m_cleanup_system;
  LevelLoaderSystem m_level_loader_system;
  HUD               m_hud;
  bool              m_restart_level;
  Video&            m_video;
  Video&            m_video2;
};
