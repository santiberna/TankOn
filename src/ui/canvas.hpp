#pragma once
#include <ui/ui_element.hpp>
#include <engine/renderer.hpp>
#include <vector>

struct Canvas
{
    std::vector<std::unique_ptr<UIElement>> elements {};

    void RenderCanvas(Renderer& renderer)
    {
        glm::vec2 screen_size = { renderer.GetScreenWidth(), renderer.GetScreenHeight() };

        DrawInfo initial {
            screen_size * 0.5f,
            screen_size,
            colour::WHITE
        };

        for (auto& root : elements)
        {
            DrawElement(renderer, *root, initial);
        }
    }

    void DrawElement(Renderer& renderer, const UIElement& element, const DrawInfo& parent_info)
    {
        if (!element.active)
            return;

        glm::vec2 new_size = parent_info.rect_size * element.transform.size;
        glm::vec2 new_center = parent_info.rect_center + (parent_info.rect_size * (element.transform.position * 2.0f - 1.0f)) * 0.5f;
        glm::vec2 pivot_correct = new_center + (new_size * (-element.transform.pivot + 0.5f));

        DrawInfo current_draw_info {
            pivot_correct,
            new_size,
            parent_info.node_colour * element.colour
        };

        {
            element.Draw(renderer, current_draw_info);
        }

        for (auto& child : element.children)
        {
            DrawElement(renderer, *child, current_draw_info);
        }
    }
};