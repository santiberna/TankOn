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

    void QueueStart()
    {
        auto buffer = asio::buffer(&read_buf.header, sizeof(Message::Header));
        asio::async_read(socket, buffer, [this](std::error_code e, size_t s)
            { HandleHeaderRead(e, s); });
    }

    void Post(const Message& msg)
    {
        asio::post(socket.get_executor(), [this, msg]
            { HandleMessagePost(msg); });
    }

    TSDeque<Message>& GetMessages() { return messages_in; }
    bool IsOpen() const { return socket.is_open(); }

private:
    void HandleMessagePost(const Message& msg)
    {
        bool writing = !messages_out.empty();
        messages_out.push_back(msg);

        if (!writing)
        {
            auto buffer = asio::buffer(&messages_out.front().header, sizeof(Message::Header));
            asio::async_write(socket, buffer, [this](std::error_code e, size_t s)
                { HandleHeaderWrite(e, s); });
        }
    }

    void HandleHeaderWrite(std::error_code ec, size_t)
    {
        if (ec)
        {
            socket.close();
            return;
        }

        auto header = messages_out.front().header;

        if (header.size > 0)
        {
            auto buffer = asio::buffer(messages_out.front().body.data);
            asio::async_write(socket, buffer, [this](std::error_code e, size_t s)
                { HandleBodyWrite(e, s); });
        }
        else
        {
            messages_out.pop_front();

            if (messages_out.count() > 0)
            {
                auto buffer = asio::buffer(&messages_out.front().header, sizeof(Message::Header));
                asio::async_write(socket, buffer, [this](std::error_code e, size_t s)
                    { HandleHeaderWrite(e, s); });
            }
        }
    }

    void HandleBodyWrite(std::error_code ec, size_t)
    {
        if (ec)
        {
            socket.close();
            return;
        }

        messages_out.pop_front();

        if (messages_out.count() > 0)
        {
            auto buffer = asio::buffer(&messages_out.front().header, sizeof(Message::Header));
            asio::async_write(socket, buffer, [this](std::error_code e, size_t s)
                { HandleHeaderWrite(e, s); });
        }
    }

    void HandleHeaderRead(std::error_code ec, size_t)
    {
        if (ec)
        {
            socket.close();
            return;
        }

        read_buf.body.data.resize(read_buf.header.size);

        if (read_buf.header.size > 0)
        {
            auto buffer = boost::asio::buffer(read_buf.body.data);
            asio::async_read(socket, buffer, [this](std::error_code e, size_t s)
                { HandleBodyRead(e, s); });
        }
        else
        {
            messages_in.push_back(read_buf);
            auto buffer = asio::buffer(&read_buf.header, sizeof(Message::Header));

            asio::async_read(socket, buffer, [this](std::error_code e, size_t s)
                { HandleHeaderRead(e, s); });
        }
    }

    void HandleBodyRead(std::error_code ec, size_t)
    {
        if (ec)
        {
            socket.close();
            return;
        }

        messages_in.push_back(read_buf);
        auto buffer = asio::buffer(&read_buf.header, sizeof(Message::Header));

        asio::async_read(socket, buffer, [this](std::error_code e, size_t s)
            { HandleHeaderRead(e, s); });
    }

    asio::ip::tcp::socket socket;

    Message read_buf {};
    TSDeque<Message> messages_out {};
    TSDeque<Message> messages_in {};
};