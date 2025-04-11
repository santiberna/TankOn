#include <resources/texture.hpp>

struct AtlasView
{
    Texture* owning_texture {};
    SDL_Rect rect {};
};

class AtlasTexture
{
public:
    static AtlasTexture FromTextureAndRects(std::shared_ptr<Texture> texture, std::vector<SDL_Rect>&& rects)
    {
        AtlasTexture out {};
        out.atlas = texture;
        out.rects = std::move(rects);
        return out;
    }

    std::optional<AtlasView> GetSprite(size_t index) const
    {
        if (index >= rects.size())
            return std::nullopt;

        AtlasView out {};
        out.owning_texture = atlas.get();
        out.rect = rects.at(index);

        return out;
    }

private:
    std::shared_ptr<Texture> atlas {};
    std::vector<SDL_Rect> rects {};
};