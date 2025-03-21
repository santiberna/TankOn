
#include <ui/core/menu.hpp>

void Menu::RenderCanvas(Renderer& renderer, const UICursorInfo& cursor_params)
{
    glm::vec2 frame_size = glm::vec2(WORLD_MAGNIFICATION * renderer.GetFrameAspectRatio(), WORLD_MAGNIFICATION);

    UIDrawInfo initial {
        frame_size * 0.5f,
        frame_size,
        colour::WHITE
    };

    // loop through all root ui elements
    for (ElementIterator it = elements.begin(); it != elements.end(); it = elements.next_sibling(it))
    {
        // Log("Drawing Root Element");
        DrawElement(renderer, it, initial, cursor_params);
    }
}

void Menu::DrawElement(Renderer& renderer, ElementIterator element, const UIDrawInfo& parent_info, const UICursorInfo& cursor_params)
{
    auto& elem = **element;

    if (!elem.active)
        return;

    UIDrawInfo current_draw_info = parent_info * elem.local_transform;

    {
        elem.Draw(renderer, current_draw_info, cursor_params);

        if (renderer.IsDebugRendering())
        {
            renderer.RenderRect(
                current_draw_info.rect_center,
                current_draw_info.rect_size,
                current_draw_info.node_colour);
        }
    }

    for (ElementIterator it = elements.begin(element); it != elements.end(element); ++it)
    {
        // Log("Drawing Child Element");
        DrawElement(renderer, it, current_draw_info, cursor_params);
    }
}