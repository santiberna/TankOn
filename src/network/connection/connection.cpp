#include <network/connection/connection.hpp>

void TCPConnection::HandleMessagePost(const Message& msg)
{
    bool writing = !out_messages.empty();

    out_messages.push_back(msg);
    if (!writing)
    {
        auto buffer = asio::buffer(&out_messages.front().header, sizeof(MessageHeader));
        asio::async_write(socket, buffer, BindMember(this, &TCPConnection::HandleHeaderWrite));
    }
}

void TCPConnection::HandleHeaderWrite(std::error_code ec, size_t)
{
    if (ec)
    {
        // Log("Error: {}", ec.message());
        socket.close();
        return;
    }

    auto header = out_messages.front().header;
    // Log("Message Header Sent: {} ({} bytes)", header.message_type, header.message_size);

    if (header.message_size > 0)
    {
        auto buffer = asio::buffer(out_messages.front().body.data.data(), header.message_size);
        asio::async_write(socket, buffer, BindMember(this, &TCPConnection::HandleBodyWrite));
    }
    else
    {
        out_messages.pop_front();

        if (out_messages.count() > 0)
        {
            auto buffer = asio::buffer(&out_messages.front().header, sizeof(MessageHeader));
            asio::async_write(socket, buffer, BindMember(this, &TCPConnection::HandleHeaderWrite));
        }
    }
}

void TCPConnection::HandleBodyWrite(std::error_code ec, size_t)
{
    if (ec)
    {
        // Log("Error: {}", ec.message());
        socket.close();
        return;
    }

    // Log("Message Body Sent: {}", out_messages.front().body.data);
    out_messages.pop_front();

    if (out_messages.count() > 0)
    {
        auto buffer = asio::buffer(&out_messages.front().header, sizeof(MessageHeader));
        asio::async_write(socket, buffer, BindMember(this, &TCPConnection::HandleHeaderWrite));
    }
}

void TCPConnection::HandleHeaderRead(std::error_code ec, size_t)
{
    if (ec)
    {
        // Log("Error: {}", ec.message());
        socket.close();
        return;
    }

    // Log("Message Header Received: {} ({} bytes)", msg_buf.header.message_type, msg_buf.header.message_size);
    msg_buf.body.data.resize(msg_buf.header.message_size);

    if (msg_buf.header.message_size > 0)
    {
        auto buffer = boost::asio::buffer(msg_buf.body.data.data(), msg_buf.header.message_size);
        asio::async_read(socket, buffer, BindMember(this, &TCPConnection::HandleBodyRead));
    }
    else
    {
        messages_in.push_back({ shared_from_this(), msg_buf });
        auto buffer = asio::buffer(&msg_buf.header, sizeof(MessageHeader));
        asio::async_read(socket, buffer, BindMember(this, &TCPConnection::HandleHeaderRead));
    }
}

void TCPConnection::HandleBodyRead(std::error_code ec, size_t)
{
    if (ec)
    {
        // Log("Error: {}", ec.message());
        socket.close();
        return;
    }

    // Log("Message Body Received: {}", msg_buf.body.data);
    messages_in.push_back({ shared_from_this(), msg_buf });

    auto buffer = asio::buffer(&msg_buf.header, sizeof(MessageHeader));
    asio::async_read(socket, buffer, BindMember(this, &TCPConnection::HandleHeaderRead));
}