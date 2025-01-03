#include <game/game.hpp>
#include <SDL3/SDL_main.h>
#include <utility/imgui_common.hpp>

int main(int argc, char* args[])
{
    SDLAbortIfFailed(SDL_Init(SDL_INIT_VIDEO));

    {
        Game game {};
        imgui_shortcuts::InitSDL3(game.context.GetWindow(), game.context.GetRenderer());

        while (!game.should_quit)
        {
            imgui_shortcuts::StartFrame();

            game.ProcessAllEvents();

            switch (game.state)
            {
            case GameState::eMENU:
            {
                game.ClearScreen(glm::vec3 {});
                game.DrawMenuUI();
                break;
            }
            case GameState::eLOBBY:
            {
                break;
            }
            case GameState::eGAME:
            {
                break;
            }
            }

            imgui_shortcuts::EndFrame(game.context.GetRenderer());
            SDL_RenderPresent(game.context.GetRenderer());
        }

        imgui_shortcuts::ShutdownSDL3();
    }

    SDL_Quit();
    return 0;
}