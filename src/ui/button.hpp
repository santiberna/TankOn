#include <ui/text_box.hpp>

class Button
{
public:
    Button(std::shared_ptr<Font> font, std::shared_ptr<Texture> texture, const glm::vec2& position, const glm::vec2& size)
        : textbox(font)
        , texture(texture)
        , position(position)
        , size(size)
    {
    }

    void SetText(const Unicode::String& text) { textbox.SetText(text); }
    const Texture& GetSprite() const { return *texture; }
    const TextBox& GetTextBox() const { return textbox; }

private:
    TextBox textbox;
    std::shared_ptr<Texture> texture;

public:
    glm::vec2 position {};
    glm::vec2 size {};
};