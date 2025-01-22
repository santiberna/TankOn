#include <resources/font.hpp>
#include <utility/file_io.hpp>
#include <stb/stb_truetype.h>
#include <math/rect_pack.hpp>

std::optional<Font> Font::FromFile(SDL_Renderer* renderer, const std::string& font_file, const FontLoadInfo& load_info)
{
    std::vector<std::byte> font_data {};
    stbtt_fontinfo font {};

    // Load font file
    {
        auto file_stream = fileIO::OpenReadStream(font_file);
        if (!file_stream)
            return std::nullopt;

        font_data = fileIO::DumpStreamIntoBytes(file_stream.value());

        if (!stbtt_InitFont(&font, std::bit_cast<const uint8_t*>(font_data.data()), 0))
        {
            return std::nullopt;
        }
    }

    FontMetrics metrics {};
    float font_scale = stbtt_ScaleForPixelHeight(&font, load_info.resolutionY);

    // Get general metrics
    {
        int line_gap, ascent, descent;
        stbtt_GetFontVMetrics(&font, &ascent, &descent, &line_gap);

        metrics.resolution = load_info.resolutionY;
        metrics.ascent = (float)ascent * font_scale;
        metrics.descent = (float)descent * font_scale;
        metrics.line_gap = (float)line_gap * font_scale;
    }

    // Find all codepoints
    std::vector<unicode::Codepoint> codepoints_found {};

    for (const auto& range : load_info.codepoint_ranges)
    {
        for (uint32_t i = range.first; i < range.second; i++)
        {
            if (stbtt_FindGlyphIndex(&font, i))
            {
                codepoints_found.emplace_back(i);
            }
        }
    }

    // Collect all kerning data

    std::unordered_map<unicode::CodepointPair, float> kerning_table {};

    for (size_t i = 0; i < codepoints_found.size(); i++)
    {
        for (size_t j = 0; j < codepoints_found.size(); j++)
        {
            auto point1 = codepoints_found[i];
            auto point2 = codepoints_found[j];

            int kerning = stbtt_GetGlyphKernAdvance(&font, stbtt_FindGlyphIndex(&font, point1), stbtt_FindGlyphIndex(&font, point2));

            if (kerning != 0)
            {
                kerning_table.emplace(std::make_pair(point1, point2), (float)kerning * font_scale);
            }
        }
    }

    // Collect codepoint metrics

    std::unordered_map<unicode::Codepoint, CodepointInfo> codepoint_info {};

    for (auto codepoint : codepoints_found)
    {
        int x1, x2, y1, y2;
        stbtt_GetCodepointBitmapBox(&font, codepoint, font_scale, font_scale, &x1, &y1, &x2, &y2);

        int left_bearing, advance;
        stbtt_GetCodepointHMetrics(&font, codepoint, &advance, &left_bearing);

        CodepointInfo info {};
        info.advance = (float)advance * font_scale;
        info.left_bearing = (float)left_bearing * font_scale;
        info.offset = glm::vec2(x1, y1);
        info.size = glm::vec2(x2 - x1, y2 - y1);

        codepoint_info.emplace(codepoint, info);
    }

    // Pack all codepoint rectangles

    RectPack::PackResult packed_codepoints {};

    {
        std::vector<glm::uvec2> codepoint_rects {};
        for (auto codepoint : codepoints_found)
        {
            codepoint_rects.emplace_back(glm::uvec2(codepoint_info[codepoint].size));
        }

        packed_codepoints = RectPack::PackRectanglesUntilSuccess(
            codepoint_rects,
            load_info.initial_atlas_packing_area,
            load_info.atlas_margin);
    }

    // Render into Atlas

    Image font_atlas {};

    if (auto new_bitmap = Image::New(packed_codepoints.packing_area.x, packed_codepoints.packing_area.y))
    {
        font_atlas = std::move(new_bitmap.value());
    }

    for (size_t i = 0; i < codepoints_found.size(); i++)
    {
        auto rect = packed_codepoints.packed_rects.at(i);

        if (rect.h == 0 || rect.w == 0)
            continue;

        auto codepoint = codepoints_found[i];
        codepoint_info[codepoint].atlas_index = i;

        int width, height, x_off, y_off;
        uint8_t* render = stbtt_GetCodepointBitmap(&font, font_scale, font_scale, codepoint, &width, &height, &x_off, &y_off);

        if (render)
        {

            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    size_t char_index = j * width + i;
                    size_t atlas_index = (j + rect.y) * font_atlas.surface->w + (i + rect.x);

                    uint32_t pixel_val = SDL_MapSurfaceRGBA(font_atlas.surface.get(), 0xFF, 0xFF, 0xFF, render[char_index]);
                    font_atlas.data.at(atlas_index) = pixel_val;
                }
            }

            stbtt_FreeBitmap(render, nullptr);
        }

        for (auto pixel : font_atlas.data)
        {
            uint8_t r = pixel >> 16;
            uint8_t g = pixel >> 8;
            uint8_t b = pixel;

            assert(r == 0xFF || r == 0);
            assert(g == 0xFF || g == 0);
            assert(b == 0xFF || b == 0);
        }
    }

    Font out {};

    if (auto texture = Texture::FromImage(renderer, font_atlas))
    {
        out.font_atlas = std::move(texture.value());
    }
    else
    {
        return std::nullopt;
    }

    out.codepoint_rects = std::move(packed_codepoints.packed_rects);
    out.codepoint_data = std::move(codepoint_info);
    out.kerning_table = std::move(kerning_table);
    out.font_metrics = metrics;

    return out;
}

CodepointInfo Font::GetCodepointInfo(unicode::Codepoint codepoint) const
{
    if (auto it = codepoint_data.find(codepoint); it != codepoint_data.end())
    {
        return it->second;
    }
    return codepoint_data.find(unicode::MISSING_CODEPOINT)->second;
}

float Font::GetKerning(unicode::Codepoint first, unicode::Codepoint next) const
{
    if (auto it = kerning_table.find({ first, next }); it != kerning_table.end())
    {
        return it->second;
    }
    return 0.0f;
}