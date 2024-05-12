#pragma once

#include "Gamepad.hpp"

#include <SDL2/SDL.h>
#include <memory>

class IScene;

class SceneManager
{
public:
    SceneManager();
    static const int        kMaxSceneNum = 4; // Should be sufficient for the moment. Later convert to a template variable
    int                     PushScene(std::shared_ptr<IScene> scene);
    int                     PopScene();
    int                     IsEmpty();
    std::shared_ptr<IScene> CurrentScene();
    // TODO: Perhaps add later change_scene method

private:
    int                     m_top_index;
    std::shared_ptr<IScene> m_scene_stack[kMaxSceneNum];
};

// Interface class for all game scenes
class IScene
{
public:
    virtual ~IScene() = default;
    virtual void ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager) = 0;
    virtual void Update() = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
    // Perhaps add on-enter on-exit methods?
};
