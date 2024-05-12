#include "GameLoop.hpp"

#include <spdlog/spdlog.h>

void
RunLoop(Gamepad& gamepad, Window& window, SceneManager& scene_manager)
{
    while (window.IsOpen())
    {
        window.ProcessEvents();
        gamepad.Update(SDL_GetKeyboardState(nullptr));

        scene_manager.CurrentScene()->ProcessEvents(gamepad, scene_manager);
        
        if (scene_manager.IsEmpty())
        {
            break;
            spdlog::info("No scenes in the stack. Exiting game loop");
        }

        scene_manager.CurrentScene()->Update();
        scene_manager.CurrentScene()->Render(window.GetRenderer());
    }
}
