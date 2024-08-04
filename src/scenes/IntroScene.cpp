#include "IntroScene.hpp"

#include "../Colors.hpp"
#include "../Config.hpp"
#include "GamePlayScene.hpp"

IntroScene::IntroScene(AssetManager& asset_manager, int level)
  : m_asset_manager{asset_manager}
  , m_level{level}
  , m_font{m_asset_manager.GetFont(Config::font_l)}
  , m_imgui{} // should we pass here the gamepad???
{
}

void
IntroScene::ProcessEvents(const Gamepad& gamepad, SceneManager& scene_manager)
{
  m_imgui.UpdateGUIState(gamepad);

  if (m_imgui.Button(m_start_game_text, 20, 50)) {
    std::shared_ptr<IScene> gameplay_scene = std::make_shared<GamePlayScene>(m_asset_manager, m_level);
    scene_manager.PushScene(gameplay_scene);
  }
  if (m_imgui.Button(m_exit_text, 20, 100)) {
    scene_manager.PopScene();
  }
}

void
IntroScene::Update([[maybe_unused]] uint64_t ticks)
{
}

void
IntroScene::Render(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);

  SDL_RenderClear(renderer);

  m_imgui.Render(m_asset_manager.GetFont(Config::font_s), renderer);

  m_font.DrawText(m_game_title, 10, 10, Colors::BLACK, renderer); // TODO: Add this to the the imgui framework, i.e., a game title

  SDL_RenderPresent(renderer);
}
