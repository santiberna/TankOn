#include <string>
#include <string_view>

namespace Unicode
{

using Codepoint = char32_t;
using CodepointPair = std::pair<char32_t, char32_t>;

using String = std::basic_string<Codepoint>;
using StringView = std::basic_string_view<Codepoint>;

constexpr Codepoint SPACE_CODEPOINT = 0x20;
constexpr Codepoint LINEBREAK_CODEPOINT = 0x0A;
constexpr Codepoint MISSING_CODEPOINT = 0x3F;

constexpr std::pair<Codepoint, Codepoint> ASCII_CODESET = { 1, 128 };

inline String ASCII_to_Unicode(const std::string& string)
{
    String out {};
    out.reserve(string.size());

    for (auto c : string)
    {
        out.push_back(static_cast<Codepoint>(c));
    }

    return out;
}

}

// Relevant specializations

namespace std
{

template <>
struct hash<Unicode::CodepointPair>
{
    std::size_t operator()(const Unicode::CodepointPair& k) const noexcept
    {
        return ((size_t)(k.first) << 32) | k.second;
    }
};

template <>
struct equal_to<Unicode::CodepointPair>
{
    bool operator()(const Unicode::CodepointPair& lhs, const Unicode::CodepointPair& rhs) const noexcept
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

}