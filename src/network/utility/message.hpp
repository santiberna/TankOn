#pragma once
#include <cstdint>
#include <string>

struct Message
{
    struct Header
    {
        uint32_t type {};
        uint32_t size {};
    } header {};

    struct Body
    {
        std::string data {};
    } body {};

    Message() = default;

    Message(uint32_t type, const std::string& msg)
        : header(type, (uint32_t)msg.size())
        , body(msg)
    {
    }
};