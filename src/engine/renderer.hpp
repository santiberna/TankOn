#pragma once
#include <optional>
#include <utility/sdl_helpers.hpp>
#include <resources/texture.hpp>
#include <math/types.hpp>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

class Renderer
{
public:
    static std::optional<Renderer> Create(uint32_t window_width, uint32_t window_height);

    uint32_t GetScreenWidth() const { return window_width; }
    uint32_t GetScreenHeight() const { return window_height; }

    SDL_Renderer* GetRenderer() { return renderer.get(); }
    SDL_Window* GetWindow() { return window.get(); }

    void ClearScreen(const glm::vec3& colour);

    void RenderSprite(
        const Texture& sprite,
        const Transform2D& transform);

    void RenderSpriteRect(
        const Texture& sprite,
        const glm::vec2& screen_pos,
        const glm::vec2& screen_size,
        const glm::vec4& colour_mult);

    void RenderDebugRect(
        const glm::vec2& position,
        const glm::vec2& size,
        const glm::vec4& colour);

private:
    uint32_t window_width {};
    uint32_t window_height {};

    sdl::UniquePtr<SDL_Window, SDL_DestroyWindow> window {};
    sdl::UniquePtr<SDL_Renderer, SDL_DestroyRenderer> renderer {};
};