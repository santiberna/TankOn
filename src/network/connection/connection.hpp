#pragma once

#include <network/network_common.hpp>
#include <network/containers/ts_deque.hpp>
#include <network/containers/message.hpp>
#include <utility/log.hpp>

class TCPConnection;

struct OwnedMessage
{
    std::shared_ptr<TCPConnection> sender;
    Message message;
};

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
public:
    TCPConnection(TSDeque<OwnedMessage>& messages_in, asio::io_context& context, asio::ip::tcp::socket&& socket)
        : messages_in(messages_in)
        , context(context)
        , socket(std::move(socket))
    {
    }

    ~TCPConnection()
    {
        socket.close();
    }

    void Post(const Message& msg)
    {
        asio::post(context, [this, msg]()
            { HandleMessagePost(msg); });
    }

    asio::ip::tcp::socket& GetSocket() { return socket; }

    void StartListening()
    {
        auto buffer = boost::asio::buffer(&msg_buf.header, sizeof(MessageHeader));
        asio::async_read(socket, buffer, BindMember(this, &TCPConnection::HandleHeaderRead));
    }

    void SetId(uint32_t set_id) { id = set_id; }
    uint32_t GetId() const { return id; }

private:
    void HandleMessagePost(const Message& msg);

    void HandleHeaderWrite(std::error_code ec, size_t);
    void HandleHeaderRead(std::error_code ec, size_t);
    void HandleBodyWrite(std::error_code ec, size_t);
    void HandleBodyRead(std::error_code ec, size_t);

private:
    uint32_t id = 0;
    TSDeque<OwnedMessage>& messages_in;
    asio::io_context& context;
    asio::ip::tcp::socket socket;

    TSDeque<Message> out_messages {};
    Message msg_buf {};
};