#include <game/game.hpp>
#include <SDL3/SDL_main.h>
#include <utility/imgui_common.hpp>
#include <game/states/main_menu_state.hpp>

int main(int, char*[])
{
    SDLAbortIfFailed(SDL_Init(SDL_INIT_VIDEO));

    {
        Game game {};
        game.SetGameState(new MainMenuState());
        imgui_shortcuts::InitSDL3(game.context.GetWindow(), game.context.GetRenderer());

        while (!game.should_quit)
        {
            imgui_shortcuts::StartFrame();

            game.ProcessAllEvents();
            game.ExecuteFrame();

            imgui_shortcuts::EndFrame(game.context.GetRenderer());
            SDL_RenderPresent(game.context.GetRenderer());
        }

        imgui_shortcuts::ShutdownSDL3();
    }

    SDL_Quit();
    return 0;
}