// TODO: move this to cpp
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <codecvt>

#include <utility/unicode.hpp>

unicode::String unicode::FromASCII(const std::string& ascii)
{
    String out {};
    out.reserve(ascii.size());

    for (auto c : ascii)
    {
        out.push_back(static_cast<Codepoint>(c));
    }

    return out;
}

unicode::String unicode::FromUTF8(const std::string& utf8)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.from_bytes(utf8);
}