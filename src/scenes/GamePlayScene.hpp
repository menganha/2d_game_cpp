#pragma once

#include "../HUD.hpp"
#include "../LevelData.hpp"
#include "../LuaContext.hpp"
#include "../systems/CleanUpSystem.hpp"
#include "../systems/CollisionSystem.hpp"
#include "../systems/CombatSystem.hpp"
#include "../systems/EnemySystem.hpp"
#include "../systems/MovementSystem.hpp"
#include "../systems/RenderSystem.hpp"
#include "../systems/ScriptSystem.hpp"
#include "../systems/TextSystem.hpp"
#include "engine/AssetManager.hpp"
#include "engine/Gamepad.hpp"
#include "engine/IScene.hpp"

#include <SDL2/SDL.h>
#include <entt/fwd.hpp>

class GamePlayScene : public IScene
{
public:
  GamePlayScene(AssetManager& asset_manager, int level);
  ~GamePlayScene();
  void ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager) override;
  void Update(uint64_t ticks) override;
  void Render(SDL_Renderer* renderer) override;
  void OnEnter() override;
  void OnExit() override;

private:
  void LoadLevel();
  void OnDestroy(DestroyEvent destroy_event);
  void OnEndLevel(EndLevelEvent end_level_event);

  struct MainEntities
  {
    entt::entity player = entt::null;
    entt::entity camera = entt::null;
  };

  entt::registry   m_registry;
  entt::dispatcher m_dispatcher;
  MainEntities     m_main_entities;
  AssetManager&    m_asset_manager;
  MovementSystem   m_movement_system;
  CollisionSystem  m_collision_system;
  CombatSystem     m_combat_system;
  RenderSystem     m_render_system;
  EnemySystem      m_enemy_system;
  TextSystem       m_text_system;
  ScriptSystem     m_script_system;
  CleanUpSystem    m_cleanup_system;
  HUD              m_hud;
  LevelData        m_level_data;
  LuaContext       m_lua_context;
  bool             m_level_ended;
  int              m_current_level;
};
