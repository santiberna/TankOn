#pragma once
#include <resources/font.hpp>
#include <ui/core/node.hpp>

struct CodepointDraw
{
    glm::vec2 offset_from_top_left {};
    uint32_t atlas_index {};
};

class TextBox : public UINode
{
public:
    virtual void Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params);

    std::shared_ptr<Font> font;
    float font_size = 1.0f;
    unicode::String text {};

    glm::bvec2 center { true, true };

protected:
    std::vector<CodepointDraw> cached_layout {};
};