#include <ui/widgets/slider.hpp>
#include <math/aabb.hpp>

float DetermineTVal(float pos, float min, float max)
{
    if (max - min == 0.0f)
        return 0.0f;
    return glm::clamp((pos - min) / (max - min), 0.0f, 1.0f);
}

void Slider::Draw(Renderer& renderer, const UIDrawInfo& draw_info, const UICursorInfo& cursor_info)
{
    bool clicked_inside = cursor_info.cursor_state == CursorState::DOWN && CheckAABB(draw_info.rect_center, draw_info.rect_size, cursor_info.cursor_position);

    if (clicked_inside)
    {
        active = true;
    }
    else if (cursor_info.cursor_state == CursorState::RELEASED)
    {
        active = false;
    }

    if (!active)
        return;

    auto min = draw_info.rect_center - draw_info.rect_size * 0.5f;
    auto max = draw_info.rect_center + draw_info.rect_size * 0.5f;

    if (is_vertical)
    {
        float range_val = DetermineTVal(cursor_info.cursor_position.y, min.y, max.y);
        on_value_set(*this, range_val);
    }
    else
    {
        float range_val = DetermineTVal(cursor_info.cursor_position.x, min.x, max.x);
        on_value_set(*this, range_val);
    }
}