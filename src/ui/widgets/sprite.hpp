#include <ui/ui_element.hpp>
#include <resources/texture.hpp>
#include <utility/log.hpp>

class UISprite : public UIElement
{
public:
    virtual ~UISprite() = default;

    virtual void Draw(Renderer& renderer, const DrawInfo& draw_params) const override
    {
        if (sprite)
            renderer.RenderSpriteRect(*sprite, draw_params.rect_center, draw_params.rect_size, draw_params.node_colour);
        else
            Log("[UI ERROR] no texture bound to UISprite widget");
    }

    std::shared_ptr<Texture> sprite {};
};