#include <SDL3/SDL_main.h>
#include <utility/imgui_common.hpp>
#include <engine/audio.hpp>
#include <game/application.hpp>

// constexpr std::chrono::milliseconds DEFAULT_UDP_TIMEOUT = std::chrono::milliseconds(128);
// constexpr auto DISCOVER_CHANNEL = "224.0.0.2";
// constexpr uint16_t DISCOVER_PORT = 32101;
// constexpr uint16_t LISTENER_PORT = 32102;

int main(int, char*[])
{
    SDLAbortIfFailed(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));
    SDLAbortIfFailed(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG));
    SDLAbortIfFailed(Mix_OpenAudio(0, nullptr));

    {
        Application game {};
        imgui_shortcuts::InitSDL3(game.renderer.GetWindow(), game.renderer.GetRenderer());

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