#pragma once
#include <optional>
#include <string>
#include <SDL3/SDL_render.h>
#include <utility/helpers.hpp>
#include <vector>
#include <glm/vec2.hpp>

// RGBA 8 bits per channel
class Image
{
public:
    static std::optional<Image> FromFile(const std::string& path);

    std::vector<std::byte> data {};
    hpp::unique_ptr<SDL_Surface, SDL_DestroySurface> surface;
};

// RGBA 8 bits per channel
class Texture
{
public:
    static std::optional<Texture> FromImage(SDL_Renderer* renderer, const Image& image);
    glm::vec2 GetSpriteSize() const;

    hpp::unique_ptr<SDL_Texture, SDL_DestroyTexture> handle {};
};
