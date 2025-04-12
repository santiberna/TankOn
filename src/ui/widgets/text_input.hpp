#pragma once
#include <ui/widgets/text_box.hpp>
#include <input/event_system.hpp>
#include <math/aabb.hpp>

class TextInput : public TextBox
{
public:
    virtual void Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params);

    signals::signal<void(TextInput&, bool)> on_click;
    bool selected = false;

    float cursor_highlight_timer {};
    float cursor_highlight_switch { 500.0f };
    bool cursor_highlight = false;
};