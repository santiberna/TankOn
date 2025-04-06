#include <utility/unicode.hpp>
#include <convert.hpp>

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
    auto copy = utf8;
    unicode::String out {};

    while (copy.size())
    {
        uint32_t utf32 {};
        auto bytes_consumed = c_utf8::capi::c_utf8_buf_to_utf32_char_b(&utf32, copy.data(), nullptr);
        out.push_back(utf32);

        copy.erase(copy.begin(), copy.begin() + bytes_consumed);
    }

    return out;
}