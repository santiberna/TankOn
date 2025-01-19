#include <ui/text_box.hpp>

void TextBox::SetText(const Unicode::String& text)
{
    codepoint_sequence.clear();

    auto font_metrics = font.GetFontMetrics();
    total_size.y = font_metrics.ascent - font_metrics.descent;

    float current_offset = 0.0f;

    for (size_t i = 0; i < text.size(); i++)
    {
        auto codepoint = text[i];
        auto glyph = font.GetCodepointInfo(codepoint);

        float kerning {};

        if (i == text.size() - 1)
        {
            kerning = 0.0f;
        }
        else
        {
            kerning = font.GetKerning(codepoint, text[i + 1]);
        }

        float next_pos = glyph.advance + kerning;

        glm::vec2 glyph_draw_offset = {
            glyph.left_bearing + current_offset,
            -(glyph.size.y + glyph.offset.y)
        };

        codepoint_sequence.emplace_back(glyph_draw_offset, glyph.atlas_index);
        current_offset += next_pos;
    }

    total_size.x = current_offset;
}