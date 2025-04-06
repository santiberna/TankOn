#pragma once
#include <resources/font.hpp>
#include <ui/core/node.hpp>

class TextBox : public UINode
{
public:
    virtual void Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params);

    std::shared_ptr<Font> font;
    unicode::String text {};
    float font_size {};
};