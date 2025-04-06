#include <ui/widgets/text_input.hpp>

void TextInput::Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params)
{
    if (cursor_params.cursor_state == CursorState::DOWN)
    {
        bool clicked = CheckAABB(draw_params.rect_center, draw_params.rect_size, cursor_params.cursor_position);
        on_click(*this, clicked);
    }

    if (selected && cursor_params.typed_characters.size())
    {
        if (cursor_params.typed_characters.front() == unicode::BACKSPACE_CODEPOINT)
        {
            if (text.size() > 0)
                text.pop_back();
        }
        else
        {
            text.append(cursor_params.typed_characters);
        }
    }

    TextBox::Draw(renderer, draw_params, cursor_params);
};