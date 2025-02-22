#pragma once
#include <ui/ui_element.hpp>

class UIDebugRect : public UIElement
{
public:
    virtual ~UIDebugRect() = default;

    virtual void Draw(Renderer& renderer, const DrawInfo& draw_params) const override
    {
        renderer.RenderDebugRect(draw_params.rect_center, draw_params.rect_size, draw_params.node_colour);
    }
};