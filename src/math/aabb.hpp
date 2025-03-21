#pragma once
#include <math/types.hpp>
#include <SDL3/SDL_rect.h>

// Performs AABB checking with a point
// AABB is centered, not top left
inline bool CheckAABB(const glm::vec2& center, const glm::vec2& size, const glm::vec2& point)
{
    glm::vec2 top_left = center - size * 0.5f;
    glm::vec2 bottom_right = center + size * 0.5f;

    bool x_bounds = point.x > top_left.x && point.x < bottom_right.x;
    bool y_bounds = point.y > top_left.y && point.y < bottom_right.y;

    return x_bounds && y_bounds;
}

inline SDL_FRect RectFromCenterAndSize(const glm::vec2& center, const glm::vec2& size)
{
    SDL_FRect dst {};

    dst.x = center.x - size.x * 0.5f;
    dst.y = center.y - size.y * 0.5f;
    dst.w = size.x;
    dst.h = size.y;

    return dst;
}