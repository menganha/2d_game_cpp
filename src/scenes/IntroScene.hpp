#pragma once

#include "../engine/AssetManager.hpp"
#include "../engine/Gamepad.hpp"
// #include "../systems/RenderSystem.hpp"
// #include "../widgets/ButtonWidget.hpp"
// #include "../widgets/WidgetContainer.hpp"
#include "../Components.hpp"
#include "../engine/Font.hpp"
#include "../engine/IScene.hpp"
#include "../IMGUI.hpp"

#include <SDL2/SDL.h>
// #include <entt/fwd.hpp>

class IntroScene : public IScene
{
public:
    IntroScene(AssetManager& asset_manager);
    void ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager) override;
    void Update() override;
    void Render(SDL_Renderer* renderer) override;

private:
    void          StupidFunction();
    AssetManager& m_asset_manager;
    Font          m_font;
    Text          m_title_text;
    IMGUI         m_imgui;

    // WidgetContainer  m_widget_container;
    // entt::registry   m_registry;
    // entt::dispatcher m_dispatcher;
    // RenderSystem     m_render_system;
    // ButtonWidget     m_return_button;
    // ButtonWidget     m_exit_button;
};
