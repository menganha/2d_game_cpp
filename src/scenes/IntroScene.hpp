#pragma once

#include "../engine/AssetManager.hpp"
#include "../engine/Gamepad.hpp"
#include "../Components.hpp"
#include "../engine/Font.hpp"
#include "../engine/IScene.hpp"
#include "../engine/IMGUI.hpp"

#include <SDL2/SDL.h>

class IntroScene : public IScene
{
public:
    IntroScene(AssetManager& asset_manager);
    void ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager) override;
    void Update(uint64_t ticks) override;
    void Render(SDL_Renderer* renderer) override;

private:
    AssetManager& m_asset_manager;
    Font          m_font;
    Text          m_title_text;
    IMGUI         m_imgui;
};
