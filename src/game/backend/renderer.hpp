#pragma once
#include <optional>
#include <utility/sdl_helpers.hpp>
#include <SDL3/SDL_init.h>
#include <resources/texture.hpp>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

constexpr int kScreenWidth { 1600 };
constexpr int kScreenHeight { 900 };

class Renderer
{
public:
    static std::optional<Renderer> Create();

    SDL_Renderer* GetRenderer() { return renderer.get(); }
    SDL_Window* GetWindow() { return window.get(); }

    void ClearScreen(uint8_t r, uint8_t g, uint8_t b);

    void DrawSprite(
        const Texture& sprite,
        const SDL_FRect& dest_rect,
        const SDL_FRect* src_rect = nullptr,
        float rotation = 0.0f);

private:
    sdl::UniquePtr<SDL_Window, SDL_DestroyWindow> window {};
    sdl::UniquePtr<SDL_Renderer, SDL_DestroyRenderer> renderer {};
};