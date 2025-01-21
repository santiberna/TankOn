#include <game/game.hpp>
#include <SDL3/SDL_main.h>
#include <utility/imgui_common.hpp>
#include <game/states/main_menu_state.hpp>
#include <utility/colours.hpp>

int main(int, char*[])
{
    SDLAbortIfFailed(SDL_Init(SDL_INIT_VIDEO));

    {
        Game game {};
        game.SetGameState(new MainMenuState());
        imgui_shortcuts::InitSDL3(game.renderer.GetWindow(), game.renderer.GetRenderer());

        auto text_font = Font::SharedFromFile(
            game.renderer.GetRenderer(),
            "C:/Windows/Fonts/arial.ttf",
            {});

        Image white = Image::New(100, 100, 0xFFFFFFFF).value();

        TextBox test_text = TextBox(text_font);
        test_text.SetText(Unicode::ASCII_to_Unicode("the brown fox jumps over the lazy dog"));

        Button button = Button(
            text_font,
            Texture::SharedFromImage(game.renderer.GetRenderer(), white),
            { 600.0f, 600.0f }, { 100.0f, 100.0f });

        button.SetText(Unicode::ASCII_to_Unicode("the brown fox jumps over the lazy dog"));

        while (!game.should_quit)
        {
            imgui_shortcuts::StartFrame();

            game.ProcessAllEvents();
            // game.ExecuteFrame();

            game.renderer.ClearScreen(colour::BLACK);
            game.renderer.RenderText(test_text, { 600.0f, 400.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
            game.renderer.RenderButton(button);

            imgui_shortcuts::EndFrame(game.renderer.GetRenderer());
            SDL_RenderPresent(game.renderer.GetRenderer());
        }

        imgui_shortcuts::ShutdownSDL3();
    }

    SDL_Quit();
    return 0;
}