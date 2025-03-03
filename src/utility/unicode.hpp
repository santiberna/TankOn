#pragma once

#include <string>
#include <string_view>

namespace unicode
{

using Codepoint = char32_t;
using CodepointPair = std::pair<char32_t, char32_t>;

using String = std::basic_string<Codepoint>;
using StringView = std::basic_string_view<Codepoint>;

constexpr Codepoint SPACE_CODEPOINT = 0x20;
constexpr Codepoint LINEBREAK_CODEPOINT = 0x0A;
constexpr Codepoint MISSING_CODEPOINT = 0x3F;
constexpr Codepoint BACKSPACE_CODEPOINT = 0x2408;

constexpr std::pair<Codepoint, Codepoint> ASCII_CODESET = { 0x0, 0x7F };
constexpr std::pair<Codepoint, Codepoint> LATIN_SUPPLEMENT_CODESET = { 0x80, 0xFF };

String FromASCII(const std::string& ascii);
String FromUTF8(const std::string& utf8);
}

// Relevant specializations

namespace std
{

template <>
struct hash<unicode::CodepointPair>
{
    std::size_t operator()(const unicode::CodepointPair& k) const noexcept
    {
        return ((size_t)(k.first) << 32) | k.second;
    }
};

template <>
struct equal_to<unicode::CodepointPair>
{
    bool operator()(const unicode::CodepointPair& lhs, const unicode::CodepointPair& rhs) const noexcept
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

}