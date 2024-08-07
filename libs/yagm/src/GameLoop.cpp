#include "GameLoop.hpp"
#include "Log.hpp"


void
RunLoop(Gamepad& gamepad, Window& window, SceneManager& scene_manager)
{
    uint64_t ticks{};
    while (window.IsOpen())
    {
        window.ProcessEvents();
        gamepad.Update(SDL_GetKeyboardState(nullptr));

        scene_manager.CurrentScene()->ProcessEvents(gamepad, scene_manager);
        
        if (scene_manager.IsEmpty())
        {
            break;
            LINFO("No scenes in the stack. Exiting game loop");
        }

        scene_manager.CurrentScene()->Update(ticks);
        scene_manager.CurrentScene()->Render(window.GetRenderer());
        ticks++;
    }
}
