#pragma once

#include "../AssetManager.hpp"
#include "../Gamepad.hpp"
#include "../systems/CollisionSystem.hpp"
#include "../systems/CombatSystem.hpp"
#include "../systems/EnemySystem.hpp"
#include "../systems/MovementSystem.hpp"
#include "../systems/RenderSystem.hpp"
#include "IScene.hpp"

#include <SDL2/SDL.h>
#include <entt/fwd.hpp>

class GamePlayScene : public IScene
{

  public:
    GamePlayScene();
    ~GamePlayScene();

    void ProcessEvents() override;
    void Update() override;
    void Render(const AssetManager& asset_manager, SDL_Renderer* renderer) override;

  private:
    void LoadLevel();
    void RestartLevel();
    void OnDeathEvent(DeathEvent death_event);

    Gamepad          m_gamepad;
    entt::entity     m_player_entity;
    entt::registry   m_registry;
    entt::dispatcher m_dispatcher;
    MovementSystem   m_movement_system;
    CollisionSystem  m_collision_system;
    CombatSystem     m_combat_system;
    RenderSystem     m_render_system;
    EnemySystem      m_enemy_system;
};
