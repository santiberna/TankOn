#include <ui/widgets/text_box.hpp>

struct CodepointDraw
{
    glm::vec2 offset_from_top_left {};
    uint32_t atlas_index {};
};

std::vector<CodepointDraw> LayoutText(const Font& font, const unicode::String& text, const glm::vec2& area_center, const glm::vec2& area_size)
{
    std::vector<CodepointDraw> out {};

    auto global_offset = area_center - area_size * 0.5f;

    auto font_metrics = font.GetFontMetrics();
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
            glyph.offset.y + font_metrics.ascent
        };

        out.emplace_back(glyph_draw_offset + global_offset, glyph.atlas_index);
        current_offset += next_pos;
    }

    return out;
}

void TextBox::Draw(Renderer& renderer, const DrawInfo& draw_params) const
{
    auto* sprite_atlas = font->GetAtlasTexture().handle.get();
    SDL_SetTextureColorModFloat(sprite_atlas, colour.x, colour.y, colour.z);
    SDL_SetTextureAlphaModFloat(sprite_atlas, colour.w);

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

        SDLAbortIfFailed(
            SDL_RenderTexture(renderer.GetRenderer(), sprite_atlas, &src_rect, &dst_rect));
    }
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