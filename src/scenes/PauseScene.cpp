#include "PauseScene.hpp"

#include "../Colors.hpp"

#include <spdlog/spdlog.h>

PauseScene::PauseScene(const AssetManager& asset_manager)
  : m_asset_manager{asset_manager}
  , m_widget_container{}
  , m_registry{}
  , m_dispatcher{}
  , m_render_system{m_registry}
  , m_return_button{"REGRESAR", 0, 100, m_registry}
  , m_exit_button{"SALIR", 0, 200, m_registry}
{
    m_return_button.Connect<&PauseScene::EndScene>(this);
    m_exit_button.Connect<&PauseScene::StupidFunction>(this);

    m_widget_container.AppendWidget(m_return_button);
    m_widget_container.AppendWidget(m_exit_button);
}

void
PauseScene::StupidFunction()
{
    spdlog::info("Estoy apretando algo en el teclado");
    RequestChangeScene(SceneType::EXIT);
}

void
PauseScene::Update()
{}

void
PauseScene::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);

    SDL_RenderClear(renderer);

    m_render_system.Update(m_asset_manager, renderer);

    SDL_RenderPresent(renderer);
}

void
PauseScene::ProcessEvents(const Gamepad& gamepad)
{
    if (gamepad.IsButtonPressed(Gamepad::UP))
        m_widget_container.MoveToPrevious();
    if (gamepad.IsButtonPressed(Gamepad::DOWN))
        m_widget_container.MoveToNext();
    if (gamepad.IsButtonPressed(Gamepad::A))
        m_widget_container.TriggerAction();
}
