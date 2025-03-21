#include <ui/widgets/sprite.hpp>

void UISprite::Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params)
{
    if (!sprite)
    {
        Log("[UI Warning] no texture bound to UISprite widget");
        return;
    }

    SDL_FRect rect = draw_params.AsRect();

    if (lock_aspect)
    {
        float image_aspect = sprite->GetSpriteSize().x / sprite->GetSpriteSize().y;

        if (image_aspect > 1.0f) // height needs to be scaled
        {
            float height = draw_params.rect_size.x / image_aspect;
            rect = RectFromCenterAndSize(draw_params.rect_center, { draw_params.rect_size.x, height });
        }
        else if (image_aspect <= 1.0f)
        {
            float width = draw_params.rect_size.y * image_aspect;
            rect = RectFromCenterAndSize(draw_params.rect_center, { width, draw_params.rect_size.y });
        }
    }

    renderer.RenderTextureRect(*sprite, rect, draw_params.node_colour, nullptr);
}