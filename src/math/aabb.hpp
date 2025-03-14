#pragma once
#include <math/types.hpp>

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