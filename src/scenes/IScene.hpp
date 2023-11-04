#pragma once

#include "../AssetManager.hpp"

#include <SDL2/SDL.h>

// Intermediate enum to decouple scenes. Scene intances only know about each other throught this simple enum
enum class SceneType
{
    NULL_SCENE = 0,
    GAMEPLAY_SCENE,
    PAUSE_SCENE,
};

// Interface class for all game scenes
class IScene
{
  public:
    virtual ~IScene() = default;
    virtual void ProcessEvents() = 0;
    virtual void Update() = 0;
    virtual void Render(const AssetManager& asset_manager, SDL_Renderer* renderer) = 0;

    bool HasEnded() { return m_has_ended; }
    bool HasRequestedChange() { return m_next_scene != SceneType::NULL_SCENE; }
    void EndScene()
    {
        m_has_ended = true;
        m_next_scene = SceneType::NULL_SCENE;
    }
    void RequestChangeScene(SceneType scene_type)
    {
        m_next_scene = scene_type;
        m_has_ended = false;
    }

  private:
    bool      m_has_ended{ false };
    SceneType m_next_scene{ SceneType::NULL_SCENE };
};
