#include <game/backend/context.hpp>

std::optional<Context> Context::Create()
{
    SDL_Window* window {};
    SDL_Renderer* renderer {};

    auto result = SDL_CreateWindowAndRenderer(
        "SDL3 Tutorial: Hello SDL3", kScreenWidth, kScreenHeight, 0,
        &window, &renderer);

    SDLCheck(result);

    Context out {};
    out.window = decltype(out.window) { window };
    out.renderer = decltype(out.renderer) { renderer };

    return out;
}