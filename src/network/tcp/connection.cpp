#include <network/tcp/connection.hpp>

void TCPConnection::QueueStart()
{
    auto buffer = asio::buffer(&read_buf.header, sizeof(Message::Header));
    asio::async_read(socket, buffer, [this](std::error_code e, size_t s)
        { HandleHeaderRead(e, s); });
}

void TCPConnection::Post(const Message& msg)
{
    asio::post(socket.get_executor(), [this, msg]
        { HandleMessagePost(msg); });
}

void TCPConnection::HandleMessagePost(const Message& msg)
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

void TCPConnection::HandleHeaderWrite(std::error_code ec, size_t)
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

void TCPConnection::HandleBodyWrite(std::error_code ec, size_t)
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

void TCPConnection::HandleHeaderRead(std::error_code ec, size_t)
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

void TCPConnection::HandleBodyRead(std::error_code ec, size_t)
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