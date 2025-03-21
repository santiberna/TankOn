#pragma once
#include <input/signals.hpp>
#include <utility/unicode.hpp>
#include <SDL3/SDL_events.h>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <utility/log.hpp>
#include <engine/renderer.hpp>

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
    InputEventSystem(SDL_Window* bound_window)
        : window(bound_window)
    {
    }

    void SetTextInput(bool val);
    void ProcessEvent(const SDL_Event& event);

    auto& OnCloseRequested() { return on_close; }
    auto& OnTextInput() { return on_text_input; }
    auto& OnButtonClick(SDL_Mousebutton button) { return on_button_click[button]; }
    auto& OnKeyPress(SDL_Keycode key) { return on_key_press[key]; }
    auto& OnMouseMove() { return on_mouse_movement; }
    auto& OnWindowResize() { return on_window_resize; }

private:
    SDL_Window* window {};

    signals::signal<void()> on_close {};
    signals::signal<void(const glm::vec2&)> on_mouse_movement {};
    signals::signal<void(const glm::uvec2&)> on_window_resize {};
    signals::signal<void(const unicode::String&)> on_text_input {};
    std::unordered_map<SDL_Mousebutton, signals::signal<void(bool)>> on_button_click {};
    std::unordered_map<SDL_Keycode, signals::signal<void(bool)>> on_key_press {};
};