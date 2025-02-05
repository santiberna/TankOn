#pragma once
#include <utility/log.hpp>
#include <network/network_common.hpp>

class TCPAcceptor
{
public:
    using AcceptCallback = std::function<void(asio::ip::tcp::socket&& new_socket)>;

    TCPAcceptor(asio::io_context& context, asio::ip::tcp::endpoint port, AcceptCallback callback)
        : acceptor(context, port)
        , on_accept_callback(callback)
    {
        acceptor.async_accept(acceptor.get_executor(),
            [this](std::error_code ec, asio::ip::tcp::socket&& socket)
            { HandleAccept(ec, std::move(socket)); });
    }

    ~TCPAcceptor()
    {
        acceptor.cancel();
    }

private:
    void HandleAccept(std::error_code ec, asio::ip::tcp::socket&& socket)
    {
        if (ec)
        {
            return;
        }

        if (on_accept_callback)
            on_accept_callback(std::move(socket));

        acceptor.async_accept(acceptor.get_executor(),
            [this](std::error_code ec, asio::ip::tcp::socket&& socket)
            { HandleAccept(ec, std::move(socket)); });
    }

    asio::ip::tcp::acceptor acceptor;
    AcceptCallback on_accept_callback {};
};