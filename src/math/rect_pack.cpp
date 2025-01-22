#include <math/rect_pack.hpp>
#include <stb/stb_rect_pack.h>

std::optional<RectPack::PackResult> RectPack::PackRectangles(const std::vector<glm::uvec2>& rectangles, const glm::uvec2& packing_area, uint32_t margin)
{
    std::vector<stbrp_rect> stb_rects {};
    stb_rects.resize(rectangles.size());

    for (size_t i = 0; i < rectangles.size(); i++)
    {
        stb_rects[i].id = (int)i;
        stb_rects[i].w = rectangles[i].x + (margin * 2);
        stb_rects[i].h = rectangles[i].y + (margin * 2);
    }

    std::vector<stbrp_node> packing_nodes;
    packing_nodes.resize(packing_area.x);

    stbrp_context pack_context {};
    stbrp_init_target(&pack_context, packing_area.x, packing_area.y, packing_nodes.data(), packing_nodes.size());

    if (stbrp_pack_rects(&pack_context, stb_rects.data(), stb_rects.size()))
    {
        std::vector<SDL_Rect> out {};
        out.resize(stb_rects.size());

        for (size_t i = 0; i < out.size(); i++)
        {
            auto& rect = stb_rects[i];

            out[i] = {
                (int)(rect.x + margin),
                (int)(rect.y + margin),
                (int)(rect.w - (margin * 2)),
                (int)(rect.h - (margin * 2))
            };
        }

        return PackResult { out, packing_area };
    }

    return std::nullopt;
}

RectPack::PackResult RectPack::PackRectanglesUntilSuccess(const std::vector<glm::uvec2>& rectangles, const glm::uvec2& packing_area, uint32_t margin)
{
    glm::uvec2 size = packing_area;
    auto out = PackRectangles(rectangles, size, margin);

    while (!out.has_value())
    {
        size *= 2;
        out = PackRectangles(rectangles, size, margin);
    }

    return out.value();
}