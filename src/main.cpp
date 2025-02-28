#include <SDL3/SDL_main.h>
#include <utility/imgui_common.hpp>
#include <engine/audio.hpp>
#include <game/application.hpp>

// Test Stuff

#include <input/event_system.hpp>
#include <ui/widgets/debug_rect.hpp>
#include <ui/widgets/sprite.hpp>
#include <ui/widgets/text_box.hpp>
#include <ui/canvas.hpp>

Canvas SetupCanvas(Renderer& renderer, InputEventSystem& events)
{
    Canvas canvas {};

    // Font
    std::shared_ptr<Font> font = Font::SharedFromFile(renderer.GetRenderer(), "assets/fonts/arial.ttf", {});

    auto element = std::make_unique<UIDebugRect>();
    element->active = true;
    element->colour = colour::WHITE;
    element->transform.size = { 0.5f, 0.5f };

    auto child_element = std::make_unique<TextBox>(font, unicode::FromASCII("Hello World!"));
    child_element->transform.size = { 1.0f, 1.0f };

    events.OnTextInput().connect([text = child_element.get()](auto& c)
        { 
            if (c.front() == unicode::BACKSPACE_CODEPOINT && text->text.size()) {
                text->text.pop_back();
            } else if (c.front() != unicode::BACKSPACE_CODEPOINT){
            text->text.append(c.c_str());
            } });

    element->children.emplace_back(std::move(child_element));
    canvas.elements.emplace_back(std::move(element));

    return canvas;
}

int main(int, char*[])
{
    SDLAbortIfFailed(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));
    SDLAbortIfFailed(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG));
    SDLAbortIfFailed(Mix_OpenAudio(0, nullptr));

    {
        Application game {};
        imgui_shortcuts::InitSDL3(game.renderer.GetWindow(), game.renderer.GetRenderer());

        SDL_StartTextInput(game.renderer.GetWindow());

        InputEventSystem events {};

        bool exit_code = false;
        signals::scoped_connection ref = events.OnCloseRequested().connect([&exit_code]()
            { exit_code = true; });

        // Music music = Music::Create("assets/music/Retro_Platforming-David_Fesliyan.mp3").value();
        // music.Start();

        auto canvas = SetupCanvas(game.renderer, events);

        while (!exit_code)
        {
            imgui_shortcuts::StartFrame();
            game.renderer.ClearScreen(colour::BLACK);

            SDL_Event event {};
            while (SDL_PollEvent(&event))
            {
                imgui_shortcuts::ProcessSDLEvent(&event);
                events.ProcessEvent(event);
            }

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