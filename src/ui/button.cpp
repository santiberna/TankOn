#include <ui/button.hpp>

void Button::UpdateButton(const glm::vec2& mouse_pos, InputState mouse_input)
{
    auto half_size = size * 0.5f;
    bool inside_x = (mouse_pos.x > position.x - half_size.x) && (mouse_pos.x < position.x + half_size.x);
    bool inside_y = (mouse_pos.y > position.y - half_size.y) && (mouse_pos.y < position.y + half_size.y);

    if (inside_x && inside_y)
    {
        if (mouse_input == InputState::ACTIVE)
        {
            state = ButtonState::PRESSED;
        }
        else if (mouse_input == InputState::RELEASED)
        {
            event();
        }
        else
        {
            state = ButtonState::HOVERED;
        }
    }
    else
    {
        state = ButtonState::DEFAULT;
    }
}