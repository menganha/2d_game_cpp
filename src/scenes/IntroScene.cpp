#include "IntroScene.hpp"

#include "../Colors.hpp"
#include "GamePlayScene.hpp"

IntroScene::IntroScene(AssetManager& asset_manager)
    : m_asset_manager{asset_manager}
    , m_font{m_asset_manager.GetFont("f30")}
    , m_title_text{"Game Title", "f30", Colors::BLACK}
    , m_imgui{} // should we pass here the gampad???
{}

void
IntroScene::ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager)
{
    m_imgui.UpdateGUIState(gamepad);

    if (m_imgui.Button("Start Game", 20, 50))
    {
        std::shared_ptr<IScene> gameplay_scene = std::make_shared<GamePlayScene>(m_asset_manager, "scripts/level1.lua");
        scene_manager.PushScene(gameplay_scene);
    }
    if (m_imgui.Button("Exit", 20, 100))
    {
        scene_manager.PopScene();
    }
}

void
IntroScene::Update([[maybe_unused]] uint64_t ticks)
{}

void
IntroScene::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);

    SDL_RenderClear(renderer);

    m_imgui.Render(m_asset_manager.GetFont("f21"), renderer);

    m_font.DrawText("GameTitle", 10, 10, Colors::BLACK, renderer);

    SDL_RenderPresent(renderer);
}
