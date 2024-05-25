#pragma once

#include "../engine/Gamepad.hpp"
#include "../engine/IMGUI.hpp"
#include "../engine/IScene.hpp"
#include "../systems/RenderSystem.hpp"

#include <SDL2/SDL.h>
#include <entt/fwd.hpp>

class PauseScene : public IScene
{
public:
    PauseScene(AssetManager& asset_manager);
    void ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager) override;
    void Update() override;
    void Render(SDL_Renderer* renderer) override;

private:
    void          StupidFunction();
    AssetManager& m_asset_manager;
    IMGUI         m_imgui;
};
