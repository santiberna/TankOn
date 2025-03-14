#pragma once
#include <math/types.hpp>

struct RectTransform
{
    glm::vec2 position { 0.5f, 0.5f };
    glm::vec2 size { 1.0f, 1.0f };
    glm::vec2 pivot { 0.5f, 0.5f };
};
