#include <game/backend/renderer.hpp>

std::optional<Renderer> Renderer::Create()
{
    SDL_Window* window {};
    SDL_Renderer* renderer {};

    auto result = SDL_CreateWindowAndRenderer(
        "SDL3 Tutorial: Hello SDL3", kScreenWidth, kScreenHeight, 0,
        &window, &renderer);

    SDLCheck(result);

    Renderer out {};
    out.window = decltype(out.window) { window };
    out.renderer = decltype(out.renderer) { renderer };

    return out;
}

void Renderer::ClearScreen(uint8_t r, uint8_t g, uint8_t b)
{
    SDL_SetRenderDrawColor(renderer.get(), r, g, b, 0xFF);
    SDLAbortIfFailed(SDL_RenderClear(renderer.get()));
}

void Renderer::DrawSprite(const Texture& sprite, const SDL_FRect& dest_rect, const SDL_FRect* src_rect, float rotation)
{
    SDLAbortIfFailed(SDL_RenderTextureRotated(
        renderer.get(),
        sprite.handle.get(),
        src_rect,
        &dest_rect,
        rotation,
        nullptr,
        SDL_FLIP_NONE));
}