#include <glm/vec2.hpp>

struct PlayerData
{
    glm::vec2 position {};
    glm::vec2 velocity {};
    uint32_t health {};
    float rotation {};
};