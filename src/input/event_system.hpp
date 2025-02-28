#pragma once
#include <input/signals.hpp>
#include <utility/unicode.hpp>
#include <SDL3/SDL_events.h>
#include <unordered_map>
#include <glm/vec2.hpp>

using SDL_Mousebutton = Uint8;

class InputEventSystem
{
public:
    void ProcessEvent(const SDL_Event& event);

    auto& OnCloseRequested() { return on_close; }
    auto& OnTextInput() { return on_text_input; }

private:
    bool text_enabled = true;
    signals::signal<void()> on_close {};
    signals::signal<void(const unicode::String&)> on_text_input {};
};