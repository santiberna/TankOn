#pragma once
#include <SDL3/SDL_events.h>
#include <unordered_map>
#include <glm/vec2.hpp>

using SDL_Mousebutton = Uint8;

enum class InputState
{
    NONE,
    PRESSED,
    RELEASED,
    ACTIVE,
};

class InputData
{
public:
    void UpdateInput();
    void ProcessEvent(const SDL_Event& event);

    InputState GetKey(SDL_Keycode code) { return key_state[code]; }
    InputState GetButton(SDL_Mousebutton code) { return button_state[code]; }

    bool ShouldClose() const { return exit_code; }
    glm::vec2 GetMousePos() const { return mouse_pos; }

private:
    std::unordered_map<SDL_Keycode, InputState> key_state {};
    std::unordered_map<SDL_Mousebutton, InputState> button_state {};

    glm::vec2 mouse_pos { -1.0f, -1.0f };
    bool exit_code = false;
};