#include <ui/widgets/button.hpp>
#include <math/aabb.hpp>

void Button::Draw(Renderer& renderer, const UIDrawInfo& draw_info, const UICursorInfo& cursor_info)
{
    bool cursor_in = CheckAABB(draw_info.rect_center, draw_info.rect_size, cursor_info.cursor_position);

    if (cursor_in == false)
    {
        state = ButtonState::DEFAULT;
    }
    else
    {
        if (state != ButtonState::PRESSED)
        {
            state = ButtonState::HOVERED;
        }

        if (cursor_info.cursor_state == CursorState::DOWN)
        {
            state = ButtonState::PRESSED;
        }

        if (cursor_info.cursor_state == CursorState::RELEASED)
        {
            on_click(*this);
            state = ButtonState::HOVERED;
        }
    }

    switch (state)
    {
    case ButtonState::DEFAULT:
        on_default(*this, cursor_info.deltatime);
        break;
    case ButtonState::HOVERED:
        on_hover(*this, cursor_info.deltatime);
        break;
    case ButtonState::PRESSED:
        on_hold(*this, cursor_info.deltatime);
        break;
    }
}
