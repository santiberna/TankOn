#include <resources/texture.hpp>
#include <stb/stb_image.h>

std::optional<Image> Image::FromFile(const std::string& path)
{
    int width {}, height {}, comp {};
    auto* result = stbi_load(path.c_str(), &width, &height, &comp, 4);

    if (result == nullptr)
    {
        Log("STB Image: {}", stbi_failure_reason());
        return std::nullopt;
    }

    Image out {};
    auto* first = std::bit_cast<std::byte*>(result);
    out.data = std::vector<std::byte>(first, first + 4 * width * height);
    stbi_image_free(result);

    SDL_Surface* surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, out.data.data(), 4 * width);
    CheckSDL(surface);

    out.surface = decltype(out.surface) { surface };
    return out;
}

std::optional<Texture> Texture::FromImage(SDL_Renderer* renderer, const Image& image)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image.surface.get());
    CheckSDL(texture);

    Texture out {};
    out.handle = decltype(out.handle) { texture };
    return out;
}

SDL_FRect Texture::GetSpriteRect(const glm::vec2& offset) const
{
    float width = static_cast<float>(handle->w);
    float height = static_cast<float>(handle->h);

    return {
        offset.x - width * 0.5f, offset.y - height * 0.5f, width, height
    };
}