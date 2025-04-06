#include <ui/core/node.hpp>
#include <input/signals.hpp>

class CheckBox : public UINode
{
public:
    CheckBox() = default;
    virtual ~CheckBox() = default;

    virtual void Draw(Renderer& renderer, const UIDrawInfo& draw_info, const UICursorInfo& cursor_info) override;

    bool state = false;
    signals::signal<void(CheckBox&, bool)> on_value_set {};
};