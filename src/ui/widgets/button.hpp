#pragma once
#include <ui/core/node.hpp>
#include <utility/colours.hpp>
#include <input/signals.hpp>

enum class ButtonState
{
    DEFAULT,
    HOVERED,
    PRESSED
};

class Button : public UINode
{
public:
    Button() = default;
    virtual ~Button() = default;

    virtual void Draw(Renderer& renderer, const UIDrawInfo& draw_info, const UICursorInfo& cursor_info) override;

    ButtonState state = ButtonState::DEFAULT;

    signals::signal<void(Button&, DeltaMS)> on_default {};
    signals::signal<void(Button&, DeltaMS)> on_hover {};
    signals::signal<void(Button&, DeltaMS)> on_hold {};
    signals::signal<void(Button&)> on_click {};
};