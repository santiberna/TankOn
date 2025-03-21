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

std::vector<CodepointDraw> LayoutText(const Font& font, const unicode::String& text, const glm::vec2& area_center, const glm::vec2& area_size)
{
    std::vector<CodepointDraw> out {};

    auto global_offset = area_center - area_size * 0.5f;
    auto font_metrics = font.GetFontMetrics();
    glm::vec2 current_offset = {};

    float next_line = (font_metrics.line_gap - font_metrics.descent + font_metrics.ascent);

    for (size_t i = 0; i < text.size(); i++)
    {
        // Get codepoint
        auto codepoint = text[i];
        auto glyph = font.GetCodepointInfo(codepoint);

        float kerning = CalcKerning(font, text, i);
        float next_pos = glyph.advance + kerning;

        glm::vec2 glyph_draw_offset = glm::vec2 {
            glyph.left_bearing,
            glyph.offset.y + font_metrics.ascent
        };

        if (current_offset.x + next_pos > area_size.x)
        {
            current_offset.x = 0.0f;
            current_offset.y += next_line;

            // Early out, no more space for characters
            if (current_offset.y + font_metrics.ascent > area_size.y)
                return out;

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
    auto& sprite_atlas = font->GetAtlasTexture();

    SDL_SetTextureColorModFloat(sprite_atlas.handle.get(),
        draw_params.node_colour.x,
        draw_params.node_colour.y,
        draw_params.node_colour.z);

    SDL_SetTextureAlphaModFloat(sprite_atlas.handle.get(), draw_params.node_colour.w);

    auto layout = LayoutText(*font, text, draw_params.rect_center, draw_params.rect_size);

    for (const auto& c : layout)
    {
        auto rect = font->GetAtlasRect(c.atlas_index);

        SDL_FRect src_rect {};
        SDL_RectToFRect(&rect, &src_rect);

        SDL_FRect dst_rect {};
        dst_rect.x = c.offset_from_top_left.x;
        dst_rect.y = c.offset_from_top_left.y;
        dst_rect.h = src_rect.h;
        dst_rect.w = src_rect.w;

        renderer.RenderTextureRect(sprite_atlas, dst_rect, colour::WHITE, &src_rect);
    }
}