#include <ui/widgets/text_box.hpp>

struct CodepointDraw
{
    glm::vec2 offset_from_top_left {};
    uint32_t atlas_index {};
};

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

std::vector<CodepointDraw> LayoutText(const Font& font, const unicode::String& text, float font_scale, const glm::vec2& area_center, const glm::vec2& area_size)
{
    std::vector<CodepointDraw> out {};

    auto global_offset = area_center - area_size * 0.5f;
    auto font_metrics = font.GetFontMetrics();
    glm::vec2 current_offset = {};

    float next_line = (font_metrics.line_gap - font_metrics.descent + font_metrics.ascent) * font_scale;

    for (size_t i = 0; i < text.size(); i++)
    {

        // Get codepoint
        auto codepoint = text[i];
        auto glyph = font.GetCodepointInfo(codepoint);

        float kerning = CalcKerning(font, text, i) * font_scale;
        float next_pos = (glyph.advance * font_scale) + kerning;

        glm::vec2 glyph_draw_offset = glm::vec2 {
            glyph.left_bearing * font_scale,
            (glyph.offset.y + font_metrics.ascent) * font_scale
        };

        if (current_offset.x + next_pos > area_size.x)
        {
            current_offset.x = 0.0f;
            current_offset.y += next_line;

            // Early out, no more space for characters
            if (current_offset.y + font_metrics.ascent * font_scale > area_size.y)
            {
                return out;
            }

            glyph_draw_offset += current_offset;
        }
        else
        {
            glyph_draw_offset += current_offset;
        }

        out.emplace_back(glyph_draw_offset + global_offset, glyph.atlas_index);
        current_offset.x += next_pos;
    }

    return out;
}

void TextBox::Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params)
{
    auto layout = LayoutText(*font, text, font_size, draw_params.rect_center, draw_params.rect_size);

    for (const auto& c : layout)
    {
        auto rect = font->GetAtlasView(c.atlas_index).value();
        auto draw_off = c.offset_from_top_left;

        SDL_FRect src_rect {};
        SDL_RectToFRect(&rect.rect, &src_rect);

        SDL_FRect dst_rect {};
        dst_rect.x = draw_off.x;
        dst_rect.y = draw_off.y;
        dst_rect.h = src_rect.h * font_size;
        dst_rect.w = src_rect.w * font_size;

        renderer.RenderTextureRect(*rect.owning_texture, dst_rect, draw_params.node_colour, &src_rect);
    }
}