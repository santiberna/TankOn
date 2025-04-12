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

std::vector<CodepointDraw> LayoutText(const Font& font, const unicode::String& text, float font_scale, const glm::vec2& area_size)
{
    std::vector<CodepointDraw> out {};

    auto font_metrics = font.GetFontMetrics();
    glm::vec2 pen_position = {};

    float line_offset = (font_metrics.line_gap - font_metrics.descent + font_metrics.ascent) * font_scale;

    auto words = SplitIntoWords(text);

    for (size_t i = 0; i < words.size(); i++)
    {
        auto& word = words[i];

        if (word.front() == unicode::LINEBREAK_CODEPOINT)
        {
            pen_position.y += line_offset;
            pen_position.x = 0.0f;
            continue;
        }

        float word_length {};

        for (size_t i = 0; i < word.length(); i++)
        {
            auto glyph = font.GetCodepointInfo(word[i]);
            float kerning = CalcKerning(font, word, i);
            word_length += (glyph.advance + kerning) * font_scale;
        }

        if (pen_position.x + word_length > area_size.x)
        {
            pen_position.y += line_offset;
            pen_position.x = 0.0f;
        }

        for (size_t i = 0; i < word.length(); i++)
        {
            auto glyph = font.GetCodepointInfo(word[i]);
            float kerning = CalcKerning(font, word, i);

            glm::vec2 glyph_draw_offset = glm::vec2 {
                glyph.left_bearing,
                (glyph.offset.y + font_metrics.ascent)
            } * font_scale;

            out.emplace_back(pen_position + glyph_draw_offset, glyph.atlas_index);
            pen_position.x += (glyph.advance + kerning) * font_scale;
        }
    }

    out.emplace_back(pen_position);
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
    cached_layout = LayoutText(*font, text, font_size, draw_params.rect_size);

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