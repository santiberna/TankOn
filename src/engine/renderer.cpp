#include <engine/renderer.hpp>

std::optional<Renderer> Renderer::Create(uint32_t window_width, uint32_t window_height)
{
    SDL_Window* window {};
    SDL_Renderer* renderer {};

    auto result = SDL_CreateWindowAndRenderer(
        "TankOn!", window_width, window_height, 0,
        &window, &renderer);

    SDLCheck(result);

    Renderer out {};
    out.window = decltype(out.window) { window };
    out.renderer = decltype(out.renderer) { renderer };

    return out;
}

void Renderer::ClearScreen(const glm::vec3& colour)
{
    SDLAbortIfFailed(SDL_SetRenderDrawColorFloat(renderer.get(), colour.x, colour.y, colour.z, 1.0f));
    SDLAbortIfFailed(SDL_RenderClear(renderer.get()));
}

void Renderer::RenderSprite(
    const Texture& sprite,
    const Transform2D& transform)
{
    float width = sprite.GetSpriteSize().x * transform.scale.x;
    float height = sprite.GetSpriteSize().y * transform.scale.y;

    SDL_FRect draw_dst {
        transform.translation.x - width * 0.5f, transform.translation.y - height * 0.5f, width, height
    };

    SDLAbortIfFailed(SDL_RenderTextureRotated(
        renderer.get(),
        sprite.handle.get(),
        nullptr,
        &draw_dst,
        glm::degrees(transform.rotation),
        nullptr,
        SDL_FLIP_NONE));
}

void Renderer::RenderSpriteRect(
    const Texture& sprite,
    const glm::vec2& screen_pos,
    const glm::vec2& screen_size,
    const glm::vec4& colour_mult)
{
    SDL_FRect draw_dst {
        screen_pos.x - screen_size.x * 0.5f, screen_pos.y - screen_size.y * 0.5f, screen_size.x, screen_size.y
    };

    SDL_SetTextureColorModFloat(sprite.handle.get(), colour_mult.x, colour_mult.y, colour_mult.z);
    SDL_SetTextureAlphaModFloat(sprite.handle.get(), colour_mult.w);

    SDLAbortIfFailed(SDL_RenderTexture(
        renderer.get(),
        sprite.handle.get(),
        nullptr,
        &draw_dst));
}

void Renderer::RenderDebugRect(
    const glm::vec2& position,
    const glm::vec2& size,
    const glm::vec4& colour)
{
    SDL_FRect rect {};
    rect.x = position.x - size.x * 0.5f;
    rect.y = position.y - size.y * 0.5f;
    rect.w = size.x;
    rect.h = size.y;

    SDLAbortIfFailed(SDL_SetRenderDrawColorFloat(renderer.get(), colour.x, colour.y, colour.z, colour.w));
    SDLAbortIfFailed(SDL_RenderRect(renderer.get(), &rect));
}