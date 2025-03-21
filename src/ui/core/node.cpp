#include <ui/core/node.hpp>

UIDrawInfo UIDrawInfo::operator*(const RectTransform& lhs) const
{
    glm::vec2 new_size = rect_size * lhs.size;
    glm::vec2 new_center = rect_center + (rect_size * (lhs.position * 2.0f - 1.0f)) * 0.5f;
    glm::vec2 pivot_correct = new_center + (new_size * (-lhs.pivot + 0.5f));

    return UIDrawInfo {
        pivot_correct,
        new_size,
        node_colour * lhs.colour
    };
}

SDL_FRect UIDrawInfo::AsRect() const
{
    SDL_FRect dst {};
    dst.x = rect_center.x - rect_size.x * 0.5f;
    dst.y = rect_center.y - rect_size.y * 0.5f;
    dst.w = rect_size.x;
    dst.h = rect_size.y;

    return dst;
}