#include <game/backend/input.hpp>

void InputData::ProcessEvent(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_EVENT_KEY_DOWN:
        key_state[e.key.key] = InputState::PRESSED;
        break;
    case SDL_EVENT_KEY_UP:
        key_state[e.key.key] = InputState::RELEASED;
        break;
    case SDL_EVENT_QUIT:
        exit_code = true;
        break;
    case SDL_EVENT_MOUSE_MOTION:
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        button_state[e.button.button] = InputState::PRESSED;
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        button_state[e.button.button] = InputState::RELEASED;
        break;
    default:
        break;
    }
}

void InputData::UpdateInput()
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