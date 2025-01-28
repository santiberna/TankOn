#include <game/game.hpp>
#include <SDL3/SDL_main.h>
#include <utility/imgui_common.hpp>
#include <utility/colours.hpp>
#include <engine/audio.hpp>

int main(int, char*[])
{
    SDLAbortIfFailed(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));
    SDLAbortIfFailed(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG));
    SDLAbortIfFailed(Mix_OpenAudio(0, nullptr));

    {
        Game game {};
        imgui_shortcuts::InitSDL3(game.renderer.GetWindow(), game.renderer.GetRenderer());

        Music music = Music::Create("assets/music/Retro_Platforming-David_Fesliyan.mp3").value();
        music.Start();

        while (!game.should_quit)
        {
            imgui_shortcuts::StartFrame();

            game.ExecuteFrame();

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

// auto& input = game.input_handler;

// button.UpdateButton(input.GetMousePos(), input.GetButton(SDL_BUTTON_LEFT));

// game.renderer.ClearScreen(colour::BLACK);
// game.renderer.RenderButton(button);

// auto text_font = Font::SharedFromFile(
//     game.renderer.GetRenderer(),
//     "C:/Windows/Fonts/arial.ttf",
//     {});

// Image white = Image::New(10, 10, 0xFF888888).value();
// auto button_texture = Texture::SharedFromImage(game.renderer.GetRenderer(), white);

// Button button = Button(text_font, button_texture, { 400.0f, 400.0f }, { 200.0f, 100.0f });
// button.SetText(unicode::FromASCII("Press to Play"));
// button.hover_mult = { 1.5f, 1.5f, 1.5f, 1.0f };
// button.press_mult = { 0.5f, 0.5f, 0.5f, 1.0f };
// button.text_colour = { 0.3f, 0.3f, 0.3f, 1.0f };

// button.SetClickEvent([]()
//     { Log("Pressed Button!"); });