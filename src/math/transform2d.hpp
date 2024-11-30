#include <glm/vec2.hpp>
#include <SDL3/SDL_rect.h>

#include <glm/mat3x3.hpp>

// Does not handle rotation
class Transform2D
{
public:
    SDL_FRect CalcSpriteDst(const glm::vec2& sprite_size)
    {
        float width = sprite_size.x * scale.x;
        float height = sprite_size.y * scale.y;

        return SDL_FRect {
            translation.x - width * 0.5f, translation.y - height * 0.5f, width, height
        };
    }

    Transform2D operator*(const Transform2D& operand)
    {
        glm::vec2 out_scale = scale * operand.scale;
        glm::vec2 out_translation = scale * operand.translation + translation;
        return { out_translation, out_scale };
    }

    glm::vec2 translation {};
    glm::vec2 scale { 1.0f, 1.0f };
};