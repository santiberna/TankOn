#include <engine/renderer.hpp>

std::optional<Renderer> Renderer::Create(uint32_t window_width, uint32_t window_height)
{
    SDL_Window* window {};
    SDL_Renderer* renderer {};

    auto result = SDL_CreateWindowAndRenderer(
        "SDL3 Tutorial: Hello SDL3", window_width, window_height, 0,
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
    const glm::vec2& screen_pos,
    const glm::vec2& scale,
    float rotation)
{
    float width = sprite.GetSpriteSize().x * scale.x;
    float height = sprite.GetSpriteSize().y * scale.y;

    SDL_FRect draw_dst {
        screen_pos.x - width * 0.5f, screen_pos.y - height * 0.5f, width, height
    };

    SDLAbortIfFailed(SDL_RenderTextureRotated(
        renderer.get(),
        sprite.handle.get(),
        nullptr,
        &draw_dst,
        rotation,
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

void Renderer::RenderText(
    const TextBox& text,
    const glm::vec2& position,
    const glm::vec4& colour)
{
    auto* sprite_atlas = text.GetFont().GetAtlasTexture().handle.get();
    SDL_SetTextureColorModFloat(sprite_atlas, colour.x, colour.y, colour.z);
    SDL_SetTextureAlphaModFloat(sprite_atlas, colour.w);

    glm::vec2 center_offset = (-text.GetTotalSize() * 0.5f) + position;

    for (const auto& c : text)
    {
        if (!c.atlas_index)
            continue;

        auto rect = text.GetFont().GetAtlasRect(c.atlas_index.value());

        SDL_FRect src_rect {};
        SDL_RectToFRect(&rect, &src_rect);

        SDL_FRect dst_rect {};
        dst_rect.x = c.offset.x + center_offset.x;
        dst_rect.y = c.offset.y + center_offset.y;
        dst_rect.h = src_rect.h;
        dst_rect.w = src_rect.w;

        SDLAbortIfFailed(
            SDL_RenderTexture(renderer.get(), sprite_atlas, &src_rect, &dst_rect));
    }
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

void Renderer::RenderButton(const Button& button)
{
    glm::vec4 colour_mult = colour::WHITE;

    switch (button.GetState())
    {
    case ButtonState::HOVERED:
        colour_mult *= button.hover_mult;
        break;
    case ButtonState::PRESSED:
        colour_mult *= button.press_mult;
        break;
    default:
        break;
    }

    RenderSpriteRect(button.GetSprite(), button.position, button.size, colour_mult);
    RenderText(button.GetTextBox(), button.position, button.text_colour * colour_mult);
}