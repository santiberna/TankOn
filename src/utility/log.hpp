#pragma once
#include <format>
#include <mutex>
#include <iostream>

namespace log_detail
{
inline std::mutex log_mutex {};
}

template <typename... Args>
void Log(std::string_view str, const Args&... args)
{
    std::scoped_lock<std::mutex> lock { log_detail::log_mutex };
    std::cout << std::vformat(str, std::make_format_args(args...)) << "\n";
    std::cout.flush();
}

template <typename... Args>
void LogW(std::wstring_view str, const Args&... args)
{
    std::scoped_lock<std::mutex> lock { log_detail::log_mutex };
    std::wcout << std::vformat(str, std::make_wformat_args(args...)) << "\n";
    std::wcout.flush();
}