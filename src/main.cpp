#include <SDL3/SDL_main.h>
#include <utility/imgui_common.hpp>
#include <engine/audio.hpp>
#include <game/application.hpp>

#include <game/data/lobby_discovery.hpp>

int main(int, char*[])
{
    Log("[INFO] Starting Game!");

    SDLAbortIfFailed(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));
    Log("[INFO] Initialized SDL Successfully!");

    SDLAbortIfFailed(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG));
    SDLAbortIfFailed(Mix_OpenAudio(0, nullptr));
    Log("[INFO] Initialized SDL_Mixer Successfully!");

    {
        Application game {};
        Log("[INFO] Initialized Game Successfully!");

        imgui_shortcuts::InitSDL3(game.renderer.GetWindow(), game.renderer.GetRenderer());
        Log("[INFO] Initialized ImGui Successfully!");

        // Music music = Music::Create("assets/music/Retro_Platforming-David_Fesliyan.mp3").value();
        // music.Start();

        while (!game.close_game)
        {
            imgui_shortcuts::StartFrame();

            game.HandleInput();
            game.DoFrame();

            imgui_shortcuts::EndFrame(game.renderer.GetRenderer());
            SDL_RenderPresent(game.renderer.GetRenderer());
        }

        imgui_shortcuts::ShutdownSDL3();
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    return 0;
}