#pragma once
#include <cstdint>
#include <string>

struct MessageHeader
{
    uint16_t message_type {};
    uint16_t message_size {};
};

struct MessageBody
{
    std::string data {};
};

struct Message
{
    Message() = default;

    Message(uint16_t type, const std::string& data)
    {
        header.message_type = type;
        header.message_size = data.size();
        body.data = data;
    }

    Message(uint16_t type)
    {
        header.message_type = type;
    }

    MessageHeader header {};
    MessageBody body {};
};