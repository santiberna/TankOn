#include <ui/widgets/button.hpp>

// void Button::UpdateButton(const glm::vec2& mouse_pos, InputState mouse_input)
// {
//     auto half_size = size * 0.5f;
//     bool inside_x = (mouse_pos.x > position.x - half_size.x) && (mouse_pos.x < position.x + half_size.x);
//     bool inside_y = (mouse_pos.y > position.y - half_size.y) && (mouse_pos.y < position.y + half_size.y);

//     if (inside_x && inside_y)
//     {
//         if (mouse_input == InputState::ACTIVE)
//         {
//             state = ButtonState::PRESSED;
//         }
//         else if (mouse_input == InputState::RELEASED)
//         {
//             event();
//         }
//         else
//         {
//             state = ButtonState::HOVERED;
//         }
//     }
//     else
//     {
//         state = ButtonState::DEFAULT;
//     }
// }

// auto& input = game.input_handler;

// button.UpdateButton(input.GetMousePos(), input.GetButton(SDL_BUTTON_LEFT));

// game.renderer.ClearScreen(colour::BLACK);
// game.renderer.RenderButton(button);

// auto text_font = Font::SharedFromFile(
//     game.renderer.GetRenderer(),
//     "C:/Windows/Fonts/arial.ttf",
//     {});

// Image white = Image::New(10, 10, 0xFF888888).value();
// auto button_texture = Texture::SharedFromImage(game.renderer.GetRenderer(), white);

// Button button = Button(text_font, button_texture, { 400.0f, 400.0f }, { 200.0f, 100.0f });
// button.SetText(unicode::FromASCII("Press to Play"));
// button.hover_mult = { 1.5f, 1.5f, 1.5f, 1.0f };
// button.press_mult = { 0.5f, 0.5f, 0.5f, 1.0f };
// button.text_colour = { 0.3f, 0.3f, 0.3f, 1.0f };

// button.SetClickEvent([]()
//     { Log("Pressed Button!"); });