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

    if (selected)
    {
        cursor_highlight_timer += cursor_params.deltatime.count();
        if (cursor_highlight_timer > cursor_highlight_switch)
        {
            cursor_highlight_timer = 0.0f;
            cursor_highlight = !cursor_highlight;
        }

        if (cursor_highlight)
        {
            auto font_metrics = font->GetFontMetrics();
            auto last_point = cached_layout.back().offset_from_top_left + draw_params.rect_center - draw_params.rect_size * 0.5f;

            glm::vec2 size = glm::vec2 { 0.0f, (font_metrics.ascent - font_metrics.descent) * font_size };
            auto margin = size * 0.02f;

            renderer.RenderLine(last_point + margin, last_point + size - margin, draw_params.node_colour);
        }
    }
    else
    {
        cursor_highlight_timer = 0.0f;
        cursor_highlight = true;
    }
};