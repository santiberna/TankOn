#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace glm
{
template <typename A>
void serialize(A& ar, vec2& self)
{
    ar(self.x);
    ar(self.y);
}
}

struct Transform2D
{
    glm::vec2 translation {};
    glm::vec2 scale { 1.0f, 1.0f };
    float rotation {};
};

namespace world
{
constexpr glm::vec2 UP = { 0.0f, -1.0f };
constexpr glm::vec2 RIGHT = { 1.0f, 0.0f };
}

namespace math
{
inline float VectorAngle(const glm::vec2& from, const glm::vec2& target)
{
    // Rotation from two unit vectors
    float cos = from.x * target.x + from.y * target.y;
    float sin = from.x * target.y + from.y * target.x;

    return std::atan2(sin, cos);
}

inline glm::vec2 AngleToVector(float angle)
{
    return { std::cosf(angle), std::sinf(angle) };
}

inline float AngleWrap(float angle)
{
    return std::fmodf(angle, 2.0f * glm::pi<float>());
}

template <typename T>
T Lerp(float t, const T& a, const T& b)
{
    return a * (1.0f - t) + b * (t);
}

}