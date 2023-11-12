#include "PauseScene.hpp"

#include "../Colors.hpp"

#include <spdlog/spdlog.h>

PauseScene::PauseScene()
  : m_widget_container{}
  , m_registry{}
  , m_dispatcher{}
  , m_render_system{ m_registry }
  , m_return_button{ "RETURN", 0, 100, m_registry }
  , m_exit_button{ "EXIT", 0, 200, m_registry }
{
    m_return_button.Connect<&PauseScene::StupidFunction>(this);
    m_exit_button.Connect<&PauseScene::StupidFunction>(this);

    m_widget_container.AppendWidget(m_return_button);
    m_widget_container.AppendWidget(m_exit_button);
}

void
PauseScene::StupidFunction()
{
    spdlog::info("Calling stupid function!");
}

void
PauseScene::Update()
{}

void
PauseScene::Render(const AssetManager& asset_manager, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);

    SDL_RenderClear(renderer);

    m_render_system.Update(asset_manager, renderer);

    SDL_RenderPresent(renderer);
}

void
PauseScene::ProcessEvents()
{
    auto keyboard_state = SDL_GetKeyboardState(nullptr);
    m_gamepad.Update(keyboard_state);
    if (m_gamepad.IsButtonPressed(Gamepad::UP))
        m_widget_container.MoveToPrevious();
    if (m_gamepad.IsButtonPressed(Gamepad::DOWN))
        m_widget_container.MoveToNext();
    if (m_gamepad.IsButtonPressed(Gamepad::A))
        m_widget_container.TriggerAction();
}
