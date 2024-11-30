#include <glm/vec2.hpp>
#include <cmath>

constexpr glm::vec2 UP = { 0.0f, -1.0f };
constexpr glm::vec2 RIGHT = { 1.0f, 0.0f };

inline float VectorAngle(const glm::vec2& from, const glm::vec2& target)
{
    // Rotation from two unit vectors
    float cos = from.x * target.x + from.y * target.y;
    float sin = from.x * target.y + from.y * target.x;

    return std::atan2(sin, cos);
}