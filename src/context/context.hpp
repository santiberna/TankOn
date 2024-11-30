#pragma once
#include <optional>
#include <utility/helpers.hpp>
#include <SDL3/SDL_init.h>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

constexpr int kScreenWidth { 1600 };
constexpr int kScreenHeight { 900 };

class Context
{
public:
    // The window we'll be rendering to
    hpp::unique_ptr<SDL_Window, SDL_DestroyWindow> window {};
    hpp::unique_ptr<SDL_Renderer, SDL_DestroyRenderer> renderer {};

public:
    static std::optional<Context> Create()
    {
        bool success = SDL_Init(SDL_INIT_VIDEO);
        CheckSDL(success);

        SDL_Window* window {};
        SDL_Renderer* renderer {};

        auto result = SDL_CreateWindowAndRenderer(
            "SDL3 Tutorial: Hello SDL3", kScreenWidth, kScreenHeight, 0,
            &window, &renderer);

        CheckSDL(result);

        Context out {};
        out.window = decltype(out.window) { window };
        out.renderer = decltype(out.renderer) { renderer };

        return out;
    }
};