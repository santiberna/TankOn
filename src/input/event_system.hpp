#pragma once
#include <input/signals.hpp>
#include <utility/unicode.hpp>
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

class InputEventSystem
{
public:
    void ProcessEvent(const SDL_Event& event);

    auto& OnCloseRequested() { return on_close; }
    auto& OnTextInput() { return on_text_input; }

    void UpdateInput();

    InputState GetKeyState(SDL_Keycode code) { return key_state[code]; }

    bool GetKey(SDL_Keycode code)
    {
        auto state = GetKeyState(code);
        return state == InputState::PRESSED || state == InputState::ACTIVE;
    }

    InputState GetButtonState(SDL_Mousebutton code) { return button_state[code]; }

    bool GetButton(SDL_Mousebutton code)
    {
        auto state = GetButtonState(code);
        return state == InputState::PRESSED || state == InputState::ACTIVE;
    }

    bool ShouldClose() const { return exit_code; }
    glm::vec2 GetMousePos() const { return mouse_pos; }

private:
    std::unordered_map<SDL_Keycode, InputState> key_state {};
    std::unordered_map<SDL_Mousebutton, InputState> button_state {};

    glm::vec2 mouse_pos { -1.0f, -1.0f };
    bool exit_code = false;

    bool text_enabled = true;
    signals::signal<void()> on_close {};
    signals::signal<void(const unicode::String&)> on_text_input {};
};