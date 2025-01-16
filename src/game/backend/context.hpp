#pragma once
#include <optional>
#include <utility/sdl_helpers.hpp>
#include <SDL3/SDL_init.h>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

constexpr int kScreenWidth { 1600 };
constexpr int kScreenHeight { 900 };

class Context
{
private:
    sdl::UniquePtr<SDL_Window, SDL_DestroyWindow> window {};
    sdl::UniquePtr<SDL_Renderer, SDL_DestroyRenderer> renderer {};

public:
    static std::optional<Context> Create();
    SDL_Renderer* GetRenderer() { return renderer.get(); }
    SDL_Window* GetWindow() { return window.get(); }
};