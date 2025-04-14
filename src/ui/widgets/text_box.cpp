#include <ui/widgets/text_box.hpp>

float CalcKerning(const Font& font, const unicode::String& text, size_t index)
{
    float out {};
    if (index == text.size() - 1)
    {
        out = 0.0f;
    }
    else
    {
        out = font.GetKerning(text[index], text[index + 1]);
    }

    return out;
}

std::vector<unicode::String> SplitIntoWords(const unicode::String& text)
{
    std::vector<unicode::String> out {};

    for (auto c : text)
    {
        if (c == unicode::SPACE_CODEPOINT)
        {
            out.emplace_back();
            out.back().push_back(unicode::SPACE_CODEPOINT);
        }
        else if (c == unicode::LINEBREAK_CODEPOINT)
        {
            out.emplace_back();
            out.back().push_back(c);
        }
        else
        {
            if (out.empty() || out.back().front() == unicode::SPACE_CODEPOINT || out.back().front() == unicode::LINEBREAK_CODEPOINT)
                out.emplace_back();

            out.back().push_back(c);
        }
    }

    return out;
}

std::vector<CodepointDraw> LayoutText(const Font& font, const unicode::String& text, float font_scale, const glm::vec2& area_size, glm::bvec2 center)
{
    std::vector<CodepointDraw> out {};

    auto font_metrics = font.GetFontMetrics();
    float line_offset = (font_metrics.line_gap - font_metrics.descent + font_metrics.ascent) * font_scale;

    if (text.empty())
    {
        glm::vec2 none {};

        if (center.x)
        {
            none.x = area_size.x * 0.5f;
        }
        if (center.y)
        {
            none.y = area_size.y * 0.5f - line_offset * 0.5f;
        }

        out.emplace_back(none);
        return out;
    }

    if (text.empty() == false)
    {
        auto words = SplitIntoWords(text);

        // First pass, organize into lines

        struct Line
        {
            unicode::String text;
            float length {};
        };

        std::vector<Line> lines {};
        lines.emplace_back();

        float line_size = 0.0f;
        for (auto& word : words)
        {
            if (word.front() == unicode::LINEBREAK_CODEPOINT)
            {
                lines.emplace_back();
                line_size = 0.0f;
                continue;
            }

            float word_length {};

            for (size_t i = 0; i < word.length(); i++)
            {
                auto glyph = font.GetCodepointInfo(word[i]);
                float kerning = CalcKerning(font, word, i);
                word_length += (glyph.advance + kerning) * font_scale;
            }

            if (line_size + word_length > area_size.x)
            {
                lines.emplace_back();
                line_size = 0.0f;
            }

            line_size += word_length;
            lines.back().text += word;
            lines.back().length = line_size;
        }

        // Second pass, draw stuff

        float vertical_center = (area_size.y - (line_offset * lines.size())) * 0.5f;

        glm::vec2 pen_position = { 0.0f, -line_offset };

        if (center.y)
        {
            pen_position.y += vertical_center;
        }

        for (auto& line : lines)
        {
            float horizontal_center = (area_size.x - line.length) * 0.5f;

            pen_position.y += line_offset;
            pen_position.x = 0.0f;

            if (center.x)
            {
                pen_position.x += horizontal_center;
            }

            for (size_t i = 0; i < line.text.size(); i++)
            {
                auto glyph = font.GetCodepointInfo(line.text[i]);
                float kerning = CalcKerning(font, line.text, i);

                glm::vec2 glyph_draw_offset = glm::vec2 {
                    glyph.left_bearing,
                    (glyph.offset.y + font_metrics.ascent)
                } * font_scale;

                out.emplace_back(pen_position + glyph_draw_offset, glyph.atlas_index);
                pen_position.x += (glyph.advance + kerning) * font_scale;
            }
        }

        out.emplace_back(pen_position);
    }

    return out;
}

void TextBox::Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params)
{
    if (font == nullptr)
    {
        Log("[UI Warn] No font assigned to textbox");
        return;
    }

    auto global_offset = draw_params.rect_center - draw_params.rect_size * 0.5f;
    cached_layout = LayoutText(*font, text, font_size, draw_params.rect_size, center);

    for (size_t i = 0; i < cached_layout.size() - 1; i++)
    {
        auto& c = cached_layout[i];
        auto rect = font->GetAtlasView(c.atlas_index).value();

        auto draw_off = global_offset + c.offset_from_top_left;

        SDL_FRect src_rect {};
        SDL_RectToFRect(&rect.rect, &src_rect);

        SDL_FRect dst_rect {};
        dst_rect.x = draw_off.x;
        dst_rect.y = draw_off.y;
        dst_rect.h = src_rect.h * font_size;
        dst_rect.w = src_rect.w * font_size;

        renderer.RenderTextureRect(*rect.owning_texture, dst_rect, draw_params.node_colour, &src_rect);

        if (renderer.IsDebugRendering())
        {
            glm::vec2 center = glm::vec2(dst_rect.x, dst_rect.y) + glm::vec2(dst_rect.w, dst_rect.h) * 0.5f;
            renderer.RenderRect(center, { dst_rect.w, dst_rect.h }, draw_params.node_colour);
        }
    }
}