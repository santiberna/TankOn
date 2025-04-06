#include <ui/core/node.hpp>
#include <input/signals.hpp>

class Slider : public UINode
{
public:
    Slider() = default;
    virtual ~Slider() = default;

    virtual void Draw(Renderer& renderer, const UIDrawInfo& draw_info, const UICursorInfo& cursor_info) override;

    // Default is horizontal slider bar
    bool is_vertical = false;
    bool active = false;
    signals::signal<void(Slider&, float)> on_value_set {};
};