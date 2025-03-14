#pragma once
#include <optional>
#include <utility/sdl_helpers.hpp>
#include <resources/texture.hpp>
#include <math/types.hpp>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

// mapping scalar from world to frame position
constexpr float WORLD_MAGNIFICATION = 1000.0f;

class Renderer
{
public:
    static std::optional<Renderer> Create(uint32_t window_width, uint32_t window_height, float aspect_ratio);

    glm::vec2 GetWindowSize() const { return { window_width, window_height }; }
    glm::vec2 GetFrameSize() const { return frame_size; }
    float GetFrameAspectRatio() const { return aspect_ratio; }

    SDL_Renderer* GetRenderer() { return renderer.get(); }
    SDL_Window* GetWindow() { return window.get(); }

    void ClearScreen(const glm::vec3& colour);

    void RenderTextureRect(
        const Texture& sprite,
        const SDL_FRect& src,
        const SDL_FRect& dst);

    void RenderSprite(
        const Texture& sprite,
        const Transform2D& transform);

    void RenderRect(
        const glm::vec2& position,
        const glm::vec2& size,
        const glm::vec4& colour);

    void SetDebugRendering(bool val) { debug_enabled = val; }
    bool IsDebugRendering() const { return debug_enabled; }

    void UpdateWindowBounds(const glm::uvec2& window_size);

    glm::vec2 WorldToScreen(const glm::vec2& world) const;
    glm::vec2 ScreenToWorld(const glm::vec2& screen) const;

private:
    bool debug_enabled = false;

    uint32_t window_width {};
    uint32_t window_height {};

    float aspect_ratio {};
    glm::vec2 frame_offset {};
    glm::vec2 frame_size {};

    sdl::UniquePtr<SDL_Window, SDL_DestroyWindow> window {};
    sdl::UniquePtr<SDL_Renderer, SDL_DestroyRenderer> renderer {};
};