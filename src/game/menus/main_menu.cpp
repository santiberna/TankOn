#include <game/menus/menus.hpp>
#include <game/application.hpp>

#include <ui/widgets/button.hpp>
#include <ui/widgets/sprite.hpp>
#include <ui/widgets/text_box.hpp>
#include <ui/widgets/text_input.hpp>

Menu MakeMainMenu(Application& app)
{
    Menu canvas {};

    // Title
    {
        auto text = std::make_unique<TextInput>();

        text->text = unicode::FromASCII("TankOn!");
        text->font = app.game_font;
        text->font_size = 7.0f;

        text->local_transform.colour = colour::WHITE;
        text->local_transform.position = { 0.5f, 0.3f };
        text->local_transform.size = { 0.5f, 0.4f };

        auto select_box = [&app](auto& self, bool click)
        {
            app.input->SetTextInput(click);
            self.selected = click;
        };

        text->on_click.connect(select_box);
        canvas.elements.insert(canvas.elements.begin(), std::move(text));
    }

    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < 2; j++)
    //     {
    //         auto region = std::make_unique<Button>();

    //         glm::vec2 pos = { (1.0 / 3.0) * i, (1.0 / 2.0) * j };
    //         glm::vec2 size = { (1.0 / 3.0), (1.0 / 2.0) };

    //         region->local_transform.position = pos + size * 0.5f;
    //         region->local_transform.size = size;

    //         auto expand = [target_size = size * 1.1f](Button& self, auto dt)
    //         {
    //             self.local_transform.size = math::Lerp(dt.count() * 0.01f, self.local_transform.size, target_size);
    //         };

    //         auto reduce = [target_size = size * 1.0f](Button& self, auto dt)
    //         {
    //             self.local_transform.size = math::Lerp(dt.count() * 0.01f, self.local_transform.size, target_size);
    //         };

    //         auto default_colour = [](Button& self, auto dt)
    //         {
    //             self.local_transform.colour = colour::WHITE;
    //         };

    //         auto hover_colour = [](Button& self, auto dt)
    //         {
    //             self.local_transform.colour = colour::LIGHT_GREY;
    //         };

    //         auto held_colour = [](Button& self, auto dt)
    //         {
    //             self.local_transform.colour = colour::GREY;
    //         };

    //         region->on_hover.connect(hover_colour);
    //         region->on_hover.connect(expand);

    //         region->on_hold.connect(held_colour);
    //         region->on_hold.connect(expand);

    //         region->on_default.connect(default_colour);
    //         region->on_default.connect(reduce);

    //         region->on_click.connect([](Button& self)
    //             { Log("Pressed"); });

    //         auto main_it = canvas.elements.insert(canvas.elements.begin(), std::move(region));

    //         auto bar = std::make_unique<UISprite>();
    //         bar->sprite = app.player_assets.begin()->health;
    //         bar->local_transform.position = { 0.5, 0.5 };
    //         bar->local_transform.pivot = { 0.5, 0.5 };
    //         bar->local_transform.size = { 1.0, 0.5 };

    //         canvas.elements.append_child(main_it, std::move(bar));
    //     }
    // }

    return canvas;
}