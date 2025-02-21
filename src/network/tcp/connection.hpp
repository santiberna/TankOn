#pragma once

#include <network/network_common.hpp>
#include <network/utility/ts_deque.hpp>
#include <network/utility/message.hpp>

class TCPConnection
{
public:
    TCPConnection(asio::ip::tcp::socket&& socket)
        : socket(std::move(socket))
    {
    }

    ~TCPConnection()
    {
        if (socket.is_open())
            socket.close();
    }

    void QueueStart();
    void Post(const Message& msg);

    TSDeque<Message>& GetMessages() { return messages_in; }
    bool IsOpen() const { return socket.is_open(); }

private:
    void HandleMessagePost(const Message& msg);
    void HandleHeaderWrite(std::error_code ec, size_t);
    void HandleBodyWrite(std::error_code ec, size_t);
    void HandleHeaderRead(std::error_code ec, size_t);
    void HandleBodyRead(std::error_code ec, size_t);

    asio::ip::tcp::socket socket;

    Message read_buf {};
    TSDeque<Message> messages_out {};
    TSDeque<Message> messages_in {};
};