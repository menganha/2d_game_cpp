#pragma once

#include "../Gamepad.hpp"
#include "../systems/RenderSystem.hpp"
#include "../widgets/ButtonWidget.hpp"
#include "../widgets/WidgetContainer.hpp"
#include "IScene.hpp"

#include <SDL2/SDL.h>
#include <entt/fwd.hpp>

class PauseScene : public IScene
{

  public:
    PauseScene();
    void ProcessEvents(const Gamepad& gamepad) override;
    void Update() override;
    void Render(const AssetManager& asset_manager, SDL_Renderer* renderer) override;

  private:
    void             StupidFunction();
    WidgetContainer  m_widget_container;
    entt::registry   m_registry;
    entt::dispatcher m_dispatcher;
    RenderSystem     m_render_system;
    ButtonWidget     m_return_button;
    ButtonWidget     m_exit_button;
};
