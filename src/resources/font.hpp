#pragma once

#include <glm/glm.hpp>
#include <utility/unicode.hpp>
#include <resources/atlas.hpp>
#include <unordered_map>

struct FontMetrics
{
    float resolution {}, ascent {}, descent {}, line_gap {};
};

struct CodepointInfo
{
    glm::vec2 offset {};
    glm::vec2 size {};
    uint32_t atlas_index {};
    float advance {};
    float left_bearing {};
};

struct FontLoadInfo
{
    std::vector<unicode::CodepointPair> codepoint_ranges {};
    glm::uvec2 initial_atlas_packing_area { 128, 128 };
    float resolutionY = 32.0f;
    uint32_t atlas_margin = 1;
};

class Font
{
public:
    static std::optional<Font> FromFile(SDL_Renderer* renderer, const std::string& font_file, const FontLoadInfo& load_params);

    static std::shared_ptr<Font> SharedFromFile(SDL_Renderer* renderer, const std::string& font_file, const FontLoadInfo& load_params)
    {
        if (auto font = FromFile(renderer, font_file, load_params))
            return std::make_shared<Font>(std::move(font.value()));
        return nullptr;
    }

    FontMetrics GetFontMetrics() const { return font_metrics; }

    std::optional<AtlasView> GetAtlasView(uint32_t index) const
    {
        return font_atlas.GetSprite(index);
    }

    CodepointInfo GetCodepointInfo(unicode::Codepoint codepoint) const;
    float GetKerning(unicode::Codepoint first, unicode::Codepoint next) const;

private:
    // Font metrics
    FontMetrics font_metrics {};
    std::unordered_map<unicode::Codepoint, CodepointInfo> codepoint_data {};
    std::unordered_map<unicode::CodepointPair, float> kerning_table {};

    // Atlas
    AtlasTexture font_atlas {};
};