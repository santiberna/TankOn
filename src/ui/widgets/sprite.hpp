#include <ui/core/node.hpp>
#include <resources/texture.hpp>
#include <utility/log.hpp>
#include <math/aabb.hpp>

class UISprite : public UINode
{
public:
    virtual ~UISprite() = default;
    virtual void Draw(Renderer& renderer, const UIDrawInfo& draw_params, const UICursorInfo& cursor_params) override;

    std::shared_ptr<Texture> sprite {};
    bool lock_aspect = true;
};