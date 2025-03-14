#pragma once
#include <resources/font.hpp>
#include <ui/widgets/text_box.hpp>
#include <input/event_system.hpp>
#include <math/aabb.hpp>

class TextInput : public TextBox
{
public:
    TextInput(InputEventSystem& input, std::shared_ptr<Font> font, const unicode::String& text)
        : TextBox(font, text)
    {
        auto button_handler = [this, &input](bool pressed)
        {
            if (!pressed)
                return;

            if (CheckAABB(input.GetMousePos(), cached_draw_info.rect_size, cached_draw_info.rect_center))
            {
                input.SetTextInput(true);
            }
            else
            {
                input.SetTextInput(false);
            }
        };

        auto text_handler = [this](auto& c)
        {
            if (c.front() == unicode::BACKSPACE_CODEPOINT && this->text.size())
            {
                this->text.pop_back();
            }
            else if (c.front() != unicode::BACKSPACE_CODEPOINT)
            {
                this->text.append(c.c_str());
            }
        };

        text_slot = input.OnTextInput().connect(text_handler);
        button_slot = input.OnButtonClick(SDL_BUTTON_LEFT).connect(button_handler);
    }

    virtual void Draw(Renderer& renderer, const DrawInfo& draw_params) const
    {
        cached_draw_info = draw_params;
        TextBox::Draw(renderer, draw_params);
    };

protected:
    mutable DrawInfo cached_draw_info {};
    signals::scoped_connection button_slot {};
    signals::scoped_connection text_slot {};
};