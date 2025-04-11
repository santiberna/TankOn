#include <game/menus/menus.hpp>
#include <game/application.hpp>

#include <ui/widgets/button.hpp>
#include <ui/widgets/sprite.hpp>
#include <ui/widgets/text_box.hpp>
#include <ui/widgets/text_input.hpp>
#include <ui/widgets/slider.hpp>
#include <ui/widgets/checkbox.hpp>

Menu MakeSettingsMenu(Application& app)
{
    Menu canvas {};

    // Slider
    {
        auto slider = std::make_unique<Slider>();

        slider->local_transform.colour = colour::WHITE;
        slider->local_transform.position = { 0.5f, 0.5f };
        slider->local_transform.size = { 0.5f, 0.1f };

        // Knob

        auto knob = std::make_unique<UISprite>();
        knob->sprite = Texture::SharedFromImage(app.renderer.GetRenderer(), Image::FromFile("assets/images/UI/White1x1.png").value());
        knob->local_transform.pivot = { 0.5f, 0.5f };
        knob->local_transform.size *= 1.2f;
        knob->local_transform.colour = colour::WHITE;

        // Bar

        auto bar = std::make_unique<UISprite>();
        bar->sprite = knob->sprite;
        bar->lock_aspect = false;
        bar->local_transform.position.x = 0.0f;
        bar->local_transform.pivot = { 0.0f, 0.5f };
        bar->local_transform.colour = colour::LIGHT_GREY;

        // Callbacks

        auto move_knob = [knob = knob.get()](Slider& slider, float val)
        {
            knob->local_transform.position.x = glm::clamp(val, 0.025f, 0.975f);
        };

        auto scale_bar = [bar = bar.get()](Slider& slider, float val)
        {
            bar->local_transform.size.x = val;
        };

        slider->on_value_set.connect(move_knob);
        slider->on_value_set.connect(scale_bar);

        slider->on_value_set(*slider, 0.5f);

        auto parent = canvas.elements.insert(canvas.elements.begin(), std::move(slider));
        canvas.elements.append_child(parent, std::move(bar));
        canvas.elements.append_child(parent, std::move(knob));
    }

    // Checkbox
    {
        // Panel
        auto checkbox_panel = std::make_unique<UINode>();
        checkbox_panel->local_transform.position = { 0.3f, 0.2f };
        checkbox_panel->local_transform.size = glm::vec2(0.09f, 0.16f * 0.33f) * 1.5f;

        // Checkbox
        auto checkbox = std::make_unique<CheckBox>();
        checkbox->local_transform.size = { 0.2f, 0.2f * 3.0f };
        checkbox->local_transform.position = { 0.15, 0.5f };

        // Text
        auto text = std::make_unique<TextBox>();
        text->font = app.game_font;
        text->font_size = 0.7f;
        text->local_transform.pivot = { 0.0f, 0.5f };
        text->local_transform.size = { 0.7f, 0.3f };
        text->local_transform.position = { 0.3f, 0.5f };
        text->text = unicode::FromASCII("Toggle debug lines");

        // Outline

        auto outline = std::make_unique<UISprite>();
        outline->sprite = Texture::SharedFromImage(app.renderer.GetRenderer(), Image::FromFile("assets/images/UI/WhiteSquare.png").value());
        SDLAbortIfFailed(SDL_SetTextureScaleMode(outline->sprite->handle.get(), SDL_ScaleMode::SDL_SCALEMODE_NEAREST));

        // Full

        auto full = std::make_unique<UISprite>();
        full->sprite = Texture::SharedFromImage(app.renderer.GetRenderer(), Image::FromFile("assets/images/UI/White1x1.png").value());

        // Callbacks

        auto show_hide = [full = full.get()](auto&, bool v)
        {
            if (v)
            {
                full->local_transform.size = { 0.7f, 0.7f };
            }
            else
            {
                full->local_transform.size = { 0.0f, 0.0f };
            }
        };

        auto enable_debug_ui_render = [&app](auto&, bool v)
        {
            app.renderer.SetDebugRendering(v);
        };

        checkbox->on_value_set.connect(show_hide);
        checkbox->on_value_set.connect(enable_debug_ui_render);

        checkbox->on_value_set(*checkbox.get(), true);

        auto parent = canvas.elements.insert(canvas.elements.begin(), std::move(checkbox_panel));
        canvas.elements.append_child(parent, std::move(text));

        auto check = canvas.elements.append_child(parent, std::move(checkbox));
        canvas.elements.append_child(check, std::move(outline));
        canvas.elements.append_child(check, std::move(full));
    }

    // Button Test

    {
        auto region = std::make_unique<Button>();

        glm::vec2 pos = { 0.5f, (1.0 / 6.0) * 2 + 0.5f };
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

        region->on_click.connect([&app](Button&)
            { app.menu_stack.pop(); });

        auto main_it = canvas.elements.insert(canvas.elements.begin(), std::move(region));

        auto bar = std::make_unique<UISprite>();
        bar->sprite = Texture::SharedFromImage(app.renderer.GetRenderer(), Image::FromFile("assets/images/UI/White1x1.png").value());
        bar->lock_aspect = false;
        bar->local_transform.position = { 0.5, 0.5 };
        bar->local_transform.pivot = { 0.5, 0.5 };
        bar->local_transform.size = { 1.0, 1.0 };

        auto text = std::make_unique<TextBox>();
        text->font = app.game_font;
        text->text = unicode::FromASCII("Back to Main Menu");
        text->local_transform.colour = colour::BLACK;

        canvas.elements.append_child(main_it, std::move(bar));
        canvas.elements.append_child(main_it, std::move(text));
    }

    return canvas;
}