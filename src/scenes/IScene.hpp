#pragma once

#include "../AssetManager.hpp"
#include "../Gamepad.hpp"

#include <SDL2/SDL.h>

// Intermediate enum to decouple scenes. Scene intances only know about each other throught this simple enum
enum class SceneType
{
    NULL_SCENE = 0,
    GAMEPLAY_SCENE,
    PAUSE_SCENE,
    EXIT,
};

// Interface class for all game scenes
class IScene
{
public:
    virtual ~IScene() = default;
    virtual void ProcessEvents(const Gamepad& gamepad) = 0;
    virtual void Update() = 0;
    virtual void Render(const AssetManager& asset_manager, SDL_Renderer* renderer) = 0;

    SceneType GetNextScene() { return m_next_scene; }
    bool      HasEnded() { return m_has_ended; }
    bool      HasRequestedChange() { return m_next_scene != SceneType::NULL_SCENE; }

    void EndScene()
    {
        m_has_ended = true;
        m_next_scene = SceneType::NULL_SCENE;
    }
    void RequestChangeScene(SceneType scene_type)
    {
        m_has_ended = false;
        m_next_scene = scene_type;
    }
    void ResetSceneStatus()
    {
        m_has_ended = false;
        m_next_scene = SceneType::NULL_SCENE;
    }

private:
    bool      m_has_ended{ false };
    SceneType m_next_scene{ SceneType::NULL_SCENE };
};
