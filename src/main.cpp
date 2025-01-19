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
        imgui_shortcuts::InitSDL3(game.renderer.GetWindow(), game.renderer.GetRenderer());

        Font text_font = Font::LoadFromFile(
            game.renderer.GetRenderer(),
            "assets/fonts/CleanCafe.otf",
            { Unicode::ASCII_CODESET },
            32, 1, { 128, 128 })
                             .value();

        TextBox test_box = TextBox(text_font);

        Unicode::String text {};

        test_box.SetText(Unicode::ASCII_to_Unicode("Hello World!"));

        while (!game.should_quit)
        {
            imgui_shortcuts::StartFrame();

            game.ProcessAllEvents();
            game.ExecuteFrame();

            game.renderer.RenderText(test_box, { 600.0f, 600.0f }, 0xFFFFFFFF);
            game.renderer.RenderDebugRect({ 600.0f, 600.0f }, test_box.GetTotalSize(), 0x00FF00FF);

            imgui_shortcuts::EndFrame(game.renderer.GetRenderer());
            SDL_RenderPresent(game.renderer.GetRenderer());

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            text.push_back(char32_t('a'));
            test_box.SetText(text);
        }

        imgui_shortcuts::ShutdownSDL3();
    }

    SDL_Quit();
    return 0;
}