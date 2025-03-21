#pragma once
#include <resources/font.hpp>
#include <ui/core/node.hpp>

class TextBox : public UINode
{
public:
    TextBox(std::shared_ptr<Font> font, const unicode::String& text)
        : font(font)
        , text(text)
    {
    }

    virtual void Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params);

protected:
    std::shared_ptr<Font> font;
    unicode::String text {};
};