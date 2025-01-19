#pragma once
#include <optional>
#include <utility/sdl_helpers.hpp>
#include <SDL3/SDL_init.h>
#include <resources/texture.hpp>
#include <ui/text_box.hpp>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

enum class DrawAlignment
{
    CENTER
};

class Renderer
{
public:
    static std::optional<Renderer> Create(uint32_t window_width, uint32_t window_height);

    SDL_Renderer* GetRenderer() { return renderer.get(); }
    SDL_Window* GetWindow() { return window.get(); }

    void ClearScreen(uint8_t r, uint8_t g, uint8_t b);

    void RenderSprite(
        const Texture& sprite,
        const glm::vec2& screen_pos,
        const glm::vec2& screen_size,
        float rotation = 0.0f);

    void RenderText(
        const TextBox& text,
        const glm::vec2& position,
        uint32_t colour);

    void RenderDebugRect(
        const glm::vec2& position,
        const glm::vec2& size,
        uint32_t colour);

private:
    sdl::UniquePtr<SDL_Window, SDL_DestroyWindow> window {};
    sdl::UniquePtr<SDL_Renderer, SDL_DestroyRenderer> renderer {};
};