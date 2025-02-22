#include <ui/widgets/text_box.hpp>

void TextBox::SetText(const unicode::String& text)
{
    codepoint_sequence.clear();

    auto font_metrics = font->GetFontMetrics();
    total_size.y = font_metrics.ascent - font_metrics.descent;

    float current_offset = 0.0f;

    for (size_t i = 0; i < text.size(); i++)
    {
        auto codepoint = text[i];
        auto glyph = font->GetCodepointInfo(codepoint);

        float kerning {};

        if (i == text.size() - 1)
        {
            kerning = 0.0f;
        }
        else
        {
            kerning = font->GetKerning(codepoint, text[i + 1]);
        }

        float next_pos = glyph.advance + kerning;

        glm::vec2 glyph_draw_offset = {
            glyph.left_bearing + current_offset,
            glyph.offset.y + font_metrics.ascent
        };

        codepoint_sequence.emplace_back(glyph_draw_offset, glyph.atlas_index);
        current_offset += next_pos;
    }

    total_size.x = current_offset;
}

// void Renderer::RenderText(
//     const TextBox& text,
//     const glm::vec2& position,
//     const glm::vec4& colour)
// {
//     auto* sprite_atlas = text.GetFont().GetAtlasTexture().handle.get();
//     SDL_SetTextureColorModFloat(sprite_atlas, colour.x, colour.y, colour.z);
//     SDL_SetTextureAlphaModFloat(sprite_atlas, colour.w);

//     glm::vec2 center_offset = (-text.GetTotalSize() * 0.5f) + position;

//     for (const auto& c : text)
//     {
//         if (!c.atlas_index)
//             continue;

//         auto rect = text.GetFont().GetAtlasRect(c.atlas_index.value());

//         SDL_FRect src_rect {};
//         SDL_RectToFRect(&rect, &src_rect);

//         SDL_FRect dst_rect {};
//         dst_rect.x = c.offset.x + center_offset.x;
//         dst_rect.y = c.offset.y + center_offset.y;
//         dst_rect.h = src_rect.h;
//         dst_rect.w = src_rect.w;

//         SDLAbortIfFailed(
//             SDL_RenderTexture(renderer.get(), sprite_atlas, &src_rect, &dst_rect));
//     }
// }