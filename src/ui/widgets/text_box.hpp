#pragma once
#include <resources/font.hpp>
#include <ui/ui_element.hpp>

class TextBox : public UIElement
{
public:
    TextBox(std::shared_ptr<Font> font, const unicode::String& text)
        : font(font)
        , text(text)
    {
    }

    virtual void Draw(Renderer& renderer, const DrawInfo& draw_params) const;

    std::shared_ptr<Font> font;
    unicode::String text {};
};