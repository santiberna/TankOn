#include <resources/texture.hpp>
#include <resources/stb_image.h>

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
    SDLCheck(surface);

    out.surface = decltype(out.surface) { surface };
    return out;
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