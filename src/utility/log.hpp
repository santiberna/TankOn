#include <format>
#include <iostream>

template <typename... Args>
void Log(std::string_view str, const Args&... args)
{
    std::cout << std::vformat(str, std::make_format_args(args...)) << "\n";
}

template <typename... Args>
void Log(std::wstring_view str, const Args&... args)
{
    std::wcout << std::vformat(str, std::make_wformat_args(args...)) << "\n";
}