#include <context/input.hpp>

void InputData::ProcessEvent(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_EVENT_KEY_DOWN:
        key_state[e.key.key] = true;
        break;
    case SDL_EVENT_KEY_UP:
        key_state[e.key.key] = false;
        break;
    case SDL_EVENT_QUIT:
        exit_code = true;
        break;
    case SDL_EVENT_MOUSE_MOTION:
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        button_state[e.button.button] = true;
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        button_state[e.button.button] = false;
        break;
    default:
        break;
    }
}