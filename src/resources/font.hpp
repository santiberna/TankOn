#pragma once

#include <glm/glm.hpp>
#include <utility/unicode.hpp>
#include <resources/texture.hpp>
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
    std::vector<Unicode::CodepointPair> codepoint_ranges { Unicode::ASCII_CODESET };
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

    const Texture& GetAtlasTexture() const { return font_atlas; }

    FontMetrics GetFontMetrics() const { return font_metrics; }

    SDL_Rect GetAtlasRect(uint32_t index) const
    {
        return codepoint_rects.at(index);
    }

    CodepointInfo GetCodepointInfo(Unicode::Codepoint codepoint) const;
    float GetKerning(Unicode::Codepoint first, Unicode::Codepoint next) const;

private:
    // Font metrics
    FontMetrics font_metrics {};
    std::unordered_map<Unicode::Codepoint, CodepointInfo> codepoint_data {};
    std::unordered_map<Unicode::CodepointPair, float> kerning_table {};

    // Atlas
    std::vector<SDL_Rect> codepoint_rects {};
    Texture font_atlas {};
};