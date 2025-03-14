#pragma once
#include <ui/widgets/text_box.hpp>
#include <utility/colours.hpp>
#include <functional>

enum class ButtonState
{
    DEFAULT,
    HOVERED,
    PRESSED
};

// class Button
// {
// public:
//     Button(std::shared_ptr<Font> font, std::shared_ptr<Texture> texture, const glm::vec2& position, const glm::vec2& size)
//         : textbox(font)
//         , texture(texture)
//         , position(position)
//         , size(size)
//     {
//     }

//     // void SetText(const unicode::String& text) { textbox.SetText(text); }
//     const Texture& GetSprite() const { return *texture; }
//     const TextBox& GetTextBox() const { return textbox; }

//     void UpdateButton(const glm::vec2& mouse_pos, InputState mouse_input);
//     void SetClickEvent(std::function<void()> ev) { event = ev; }
//     ButtonState GetState() const { return state; }

// private:
//     TextBox textbox;
//     std::shared_ptr<Texture> texture;
//     std::function<void()> event;
//     ButtonState state = ButtonState::DEFAULT;

// public:
//     glm::vec4 text_colour = colour::WHITE;
//     glm::vec4 hover_mult = colour::WHITE;
//     glm::vec4 press_mult = colour::WHITE;

//     glm::vec2 position {};
//     glm::vec2 size {};
// };