#pragma once
#include <math/types.hpp>
#include <utility/colours.hpp>
#include <engine/renderer.hpp>
#include <vector>
#include <memory>

struct DrawInfo
{
    glm::vec2 rect_center {};
    glm::vec2 rect_size {};
    glm::vec4 node_colour {};
};

struct RectTransform
{
    glm::vec2 position { 0.5f, 0.5f };
    glm::vec2 size { 1.0f, 1.0f };
    glm::vec2 pivot { 0.5f, 0.5f };
};

class UIElement
{
public:
    virtual ~UIElement() = default;
    virtual void Draw(Renderer& renderer, const DrawInfo& draw_params) const = 0;

public:
    bool active = true;
    RectTransform transform {};
    glm::vec4 colour = colour::WHITE;

    std::vector<std::unique_ptr<UIElement>> children {};
};