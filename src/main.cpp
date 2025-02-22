#include <SDL3/SDL_main.h>
#include <utility/imgui_common.hpp>
#include <engine/audio.hpp>
#include <game/application.hpp>

#include <ui/widgets/debug_rect.hpp>
#include <ui/widgets/sprite.hpp>
#include <ui/canvas.hpp>

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

        Canvas canvas {};

        auto element = std::make_unique<UIDebugRect>();
        element->active = true;
        element->colour = colour::WHITE;
        element->transform.position = { 0.5f, 0.5f };
        element->transform.size = { 0.0f, 0.3f };
        element->transform.pivot = { 0.0f, 0.0f };

        auto* e = element.get();

        auto child_element = std::make_unique<UISprite>();
        child_element->transform.size = { 0.5f, 0.5f };
        child_element->sprite = Texture::SharedFromImage(game.renderer.GetRenderer(), Image::FromFile("assets/images/UI/HeartBlue.png").value());
        element->children.emplace_back(std::move(child_element));

        canvas.elements.emplace_back(std::move(element));

        while (!game.input.ShouldClose())
        {
            imgui_shortcuts::StartFrame();

            game.renderer.ClearScreen(colour::BLACK);
            game.HandleInput();

            static float dt = 0.0f;
            dt += 0.01f;
            e->transform.size.x = (std::sin(dt) * 0.5f + 0.5f) * 0.5f;
            e->colour.a = (std::sin(dt) * 0.5f + 0.5f);

            canvas.RenderCanvas(game.renderer);

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