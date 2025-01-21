#pragma once
#include <optional>
#include <string>
#include <SDL3/SDL_render.h>
#include <utility/sdl_helpers.hpp>
#include <vector>
#include <glm/vec2.hpp>

// RGBA 32 Image
class Image
{
public:
    static std::optional<Image> New(size_t width, size_t height, uint32_t default_val = 0);
    static std::optional<Image> FromData(size_t width, size_t height, std::vector<uint32_t>&& data);
    static std::optional<Image> FromFile(const std::string& path);

    void BlitFrom(const Image& source, const SDL_Rect* src_rect, const SDL_Rect* dst_rect);

    std::vector<uint32_t> data {};
    sdl::UniquePtr<SDL_Surface, SDL_DestroySurface> surface;
};

// RGBA 8 bits per channel
class Texture
{
public:
    static std::optional<Texture> FromImage(SDL_Renderer* renderer, const Image& image);

    static std::shared_ptr<Texture> SharedFromImage(SDL_Renderer* renderer, const Image& image)
    {
        if (auto texture = FromImage(renderer, image))
            return std::make_shared<Texture>(std::move(texture.value()));
        return nullptr;
    }

    glm::vec2 GetSpriteSize() const;

    sdl::UniquePtr<SDL_Texture, SDL_DestroyTexture> handle {};
};
