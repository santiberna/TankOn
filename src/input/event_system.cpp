#include <input/event_system.hpp>
#include <SDL3/SDL_clipboard.h>

void InputEventSystem::ProcessEvent(const SDL_Event& ev)
{
    switch (ev.type)
    {
    case SDL_EVENT_KEY_DOWN:
    {
        key_state[ev.key.key] = InputState::PRESSED;

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
        key_state[ev.key.key] = InputState::RELEASED;
        break;
    case SDL_EVENT_QUIT:
        on_close();
        break;
    case SDL_EVENT_MOUSE_MOTION:
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        button_state[ev.button.button] = InputState::PRESSED;
        on_button_click[ev.button.button](true);
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        button_state[ev.button.button] = InputState::RELEASED;
        on_button_click[ev.button.button](false);
        break;
    case SDL_EVENT_TEXT_INPUT:
    {
        auto string = unicode::FromUTF8(ev.text.text);
        on_text_input(string);
        break;
    }
    default:
        break;
    }
}

void InputEventSystem::UpdateInput()
{
    for (auto&& [key, state] : key_state)
    {
        if (state == InputState::PRESSED)
            state = InputState::ACTIVE;
        else if (state == InputState::RELEASED)
            state = InputState::NONE;
    }

    for (auto&& [button, state] : button_state)
    {
        if (state == InputState::PRESSED)
            state = InputState::ACTIVE;
        else if (state == InputState::RELEASED)
            state = InputState::NONE;
    }
}