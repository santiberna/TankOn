#include <game/menus/menus.hpp>
#include <game/application.hpp>

#include <ui/widgets/button.hpp>
#include <ui/widgets/sprite.hpp>
#include <ui/widgets/text_box.hpp>
#include <ui/widgets/text_input.hpp>
#include <ui/widgets/slider.hpp>
#include <ui/widgets/checkbox.hpp>

Menu MakeMainMenu(Application& app)
{
    Menu canvas {};

    // Title
    {
        auto text = std::make_unique<TextInput>();

        text->text = unicode::FromASCII("TankOn!\nHello World!");
        text->font = app.game_font;
        text->font_size = 1.5f;

        text->local_transform.colour = colour::WHITE;
        text->local_transform.position = { 0.5f, 0.3f };
        text->local_transform.size = { 0.5f, 0.4f };

        auto select_box = [&app](auto& self, bool click)
        {
            app.input->SetTextInput(click);
            self.selected = click;
        };

        text->on_click.connect(select_box);
        canvas.AddRootNode(std::move(text));
    }

    // Buttons

    unicode::String button_text[3] = {
        unicode::FromASCII("Play"),
        unicode::FromASCII("Settings"),
        unicode::FromASCII("Quit")
    };

    std::function<void(Button&)> on_click[3] {
        [](Button&) {},
        [&app](Button&)
        { app.menu_stack.push(&app.settings_menu); },
        [&app](Button&)
        { app.close_game = true; },
    };

    for (int j = 0; j < 3; j++)
    {
        auto region = std::make_unique<Button>();

        glm::vec2 pos = { 0.5f, (1.0 / 6.0) * j + 0.6f };
        glm::vec2 size = { 0.5f, (1.0 / 6.0) };

        region->local_transform.position = pos;
        region->local_transform.size = size;

        auto expand = [target_size = size * 1.1f](Button& self, auto dt)
        {
            self.local_transform.size = math::Lerp(dt.count() * 0.01f, self.local_transform.size, target_size);
        };

        auto reduce = [target_size = size * 1.0f](Button& self, auto dt)
        {
            self.local_transform.size = math::Lerp(dt.count() * 0.01f, self.local_transform.size, target_size);
        };

        auto default_colour = [](Button& self, auto dt)
        {
            self.local_transform.colour = colour::WHITE;
        };

        auto hover_colour = [](Button& self, auto dt)
        {
            self.local_transform.colour = colour::LIGHT_GREY;
        };

        auto held_colour = [](Button& self, auto dt)
        {
            self.local_transform.colour = colour::GREY;
        };

        region->on_hover.connect(hover_colour);
        region->on_hover.connect(expand);

        region->on_hold.connect(held_colour);
        region->on_hold.connect(expand);

        region->on_default.connect(default_colour);
        region->on_default.connect(reduce);

        region->on_click.connect(on_click[j]);

        auto main_it = canvas.AddRootNode(std::move(region));

        auto bar = std::make_unique<UISprite>();
        bar->sprite = Texture::SharedFromImage(app.renderer.GetRenderer(), Image::FromFile("assets/images/UI/White1x1.png").value());
        bar->lock_aspect = false;
        bar->local_transform.position = { 0.5, 0.5 };
        bar->local_transform.pivot = { 0.5, 0.5 };
        bar->local_transform.size = { 1.0, 1.0 };

        auto text = std::make_unique<TextBox>();
        text->font = app.game_font;
        text->text = button_text[j];
        text->local_transform.colour = colour::BLACK;

        canvas.AddChildNode(main_it, std::move(bar));
        canvas.AddChildNode(main_it, std::move(text));
    }

    return canvas;
}