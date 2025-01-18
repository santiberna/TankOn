#include <SDL3/SDL_rect.h>
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace RectPack
{

struct PackResult
{
    std::vector<SDL_Rect> packed_rects {};
    glm::uvec2 packing_area {};
};

// Warning: Can fail if not enough space is supplied, returning nullopt.
// A version of this function exists that retries and increases the packing range on fail
// Note: use margin to define spacing between rectangles, for preventing things such as atlas bleeding
std::optional<PackResult> PackRectangles(
    const std::vector<glm::uvec2>& rectangles,
    const glm::uvec2& packing_area,
    uint32_t margin = 0);

// Packs rectangles, but doubles the packing area if it fails and retries again
// Useful for packing in powers of 2

PackResult PackRectanglesUntilSuccess(
    const std::vector<glm::uvec2>& rectangles,
    const glm::uvec2& packing_area,
    uint32_t margin = 0);
}