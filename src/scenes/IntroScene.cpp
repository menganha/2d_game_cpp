#include "IntroScene.hpp"

#include "../Colors.hpp"
#include "spdlog/spdlog.h"

IntroScene::IntroScene(AssetManager& asset_manager)
    : m_asset_manager{asset_manager}
    , m_font{m_asset_manager.GetFont("f30")}
    , m_title_text{"Game Title", "f30", Colors::BLACK}
{}

void
IntroScene::ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager)
{
    // Design an IMGUI?
    // GUIState guistate{}
    // guistate.begin(frame):

    if (gamepad.IsButtonPressed(Gamepad::UP))
        spdlog::info("Pressing Up");
    if (gamepad.IsButtonPressed(Gamepad::A))
    {
        spdlog::info("Exiting Scene");
        scene_manager.PopScene();
    }
}
void
IntroScene::Update()
{}

void
IntroScene::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);

    SDL_RenderClear(renderer);

    m_font.DrawText("GameTitle", 10, 10, Colors::BLACK, renderer);

    SDL_RenderPresent(renderer);
}
