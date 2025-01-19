#include <resources/font.hpp>

struct CodepointDrawData
{
    glm::vec2 offset {};
    std::optional<uint32_t> atlas_index {};
};

class TextBox
{
public:
    TextBox(Font& font)
        : font(font)
    {
    }

    void SetText(const Unicode::String& text);
    glm::vec2 GetTotalSize() const { return total_size; }
    const Font& GetFont() const { return font; }

    auto begin() const { return codepoint_sequence.begin(); }
    auto end() const { return codepoint_sequence.end(); }

private:
    Font& font;
    std::vector<CodepointDrawData> codepoint_sequence {};
    glm::vec2 total_size {};
};