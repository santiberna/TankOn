#include <resources/texture.hpp>
#include <stb/stb_image.h>

std::optional<Image> Image::New(size_t width, size_t height, uint32_t default_val)
{
    return FromData(width, height, std::vector<uint32_t>(width * height, default_val));
}

std::optional<Image> Image::FromData(size_t width, size_t height, std::vector<uint32_t>&& data)
{
    Image out {};
    out.data = std::move(data);

    SDL_Surface* surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, out.data.data(), 4 * width);
    SDLCheck(surface);

    out.surface = decltype(out.surface) { surface };
    return out;
}

std::optional<Image> Image::FromFile(const std::string& path)
{
    int width {}, height {}, comp {};
    auto* result = stbi_load(path.c_str(), &width, &height, &comp, 4);

    if (result == nullptr)
    {
        Log("STB Image: {}", stbi_failure_reason());
        return std::nullopt;
    }

    auto* first = std::bit_cast<uint32_t*>(result);
    auto out = FromData(width, height, std::vector<uint32_t>(first, first + width * height));
    stbi_image_free(result);

    return out;
}

void Image::BlitFrom(const Image& image, const SDL_Rect* src_rect, const SDL_Rect* dst_rect)
{
    SDLAbortIfFailed(SDL_BlitSurface(
        image.surface.get(), src_rect, surface.get(), dst_rect));
}

std::optional<Texture> Texture::FromImage(SDL_Renderer* renderer, const Image& image)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image.surface.get());
    SDLCheck(texture);

    Texture out {};
    out.handle = decltype(out.handle) { texture };
    return out;
}

glm::vec2 Texture::GetSpriteSize() const
{
    float width = static_cast<float>(handle->w);
    float height = static_cast<float>(handle->h);

    return {
        width, height
    };
}