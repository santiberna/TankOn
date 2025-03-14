#include <input/event_system.hpp>
#include <SDL3/SDL_clipboard.h>

void InputEventSystem::ProcessEvent(const SDL_Event& ev)
{
    switch (ev.type)
    {
    case SDL_EVENT_KEY_DOWN:
    {
        if (ev.key.repeat) break;
        on_key_press[ev.key.key](true);

        bool text_enabled = SDL_TextInputActive(window);
        if (text_enabled && ev.key.key == SDLK_BACKSPACE)
        {
            on_text_input({ unicode::BACKSPACE_CODEPOINT });
        }
        if (text_enabled && ev.key.key == SDLK_V && ev.key.mod & SDL_KMOD_CTRL)
        {
            char* data = SDL_GetClipboardText();
            on_text_input(unicode::FromUTF8({ data }));
            SDL_free(data);
        }
        break;
    }
    case SDL_EVENT_KEY_UP:
        on_key_press[ev.key.key](false);
        break;
    case SDL_EVENT_QUIT:
        on_close();
        break;
    case SDL_EVENT_MOUSE_MOTION:
    {
        glm::vec2 mouse_pos{};
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        on_mouse_movement(mouse_pos);
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        on_button_click[ev.button.button](true);
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        on_button_click[ev.button.button](false);
        break;
    case SDL_EVENT_TEXT_INPUT:
    {
        auto string = unicode::FromUTF8(ev.text.text);
        on_text_input(string);
        break;
    }
    case SDL_EVENT_WINDOW_RESIZED:
    {
        if (SDL_GetWindowID(window) != ev.window.windowID) break;
        SDL_SyncWindow(window);

        on_window_resize(glm::uvec2 { ev.window.data1, ev.window.data2});
    }
    default:
        break;
    }
}

void InputEventSystem::SetTextInput(bool val) 
{
    bool prev = SDL_TextInputActive(window);

    if (val && !prev)
    {
        Log("Started Text Input");
        SDL_StartTextInput(window);
    }
    else if (!val && prev)
    {
        Log("Stopped Text Input");
        SDL_StopTextInput(window);
    }
}
