#pragma once
#include <math/types.hpp>
#include <utility/colours.hpp>
#include <engine/renderer.hpp>
#include <utility/time.hpp>

struct RectTransform
{
    glm::vec2 position { 0.5f, 0.5f };
    glm::vec2 size { 1.0f, 1.0f };
    glm::vec2 pivot { 0.5f, 0.5f };
    glm::vec4 colour = colour::WHITE;
};

struct UIDrawInfo
{
    glm::vec2 rect_center {};
    glm::vec2 rect_size {};
    glm::vec4 node_colour {};

    UIDrawInfo operator*(const RectTransform& lhs) const;
    SDL_FRect AsRect() const;
};

enum class CursorState
{
    NONE,
    DOWN,
    RELEASED,
};

struct UICursorInfo
{
    glm::vec2 cursor_position {}; // Needs to be mapped to the frame
    CursorState cursor_state {};
    DeltaMS deltatime {};
};

class UINode
{
public:
    virtual ~UINode() = default;

    virtual void Draw(
        [[maybe_unused]] Renderer& renderer,
        [[maybe_unused]] const UIDrawInfo& draw_params,
        [[maybe_unused]] const UICursorInfo& cursor_params) { };

public:
    bool active = true;
    RectTransform local_transform {};
};