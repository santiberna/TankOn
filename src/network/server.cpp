#include <network/server.hpp>

void TCPServer::HandleAccept(std::error_code ec, asio::ip::tcp::socket new_socket)
{
    if (ec)
    {
        Log("Failed to accept connection: {}", ec.message());
        return;
    }

    connections.emplace_back(std::make_shared<TCPConnection>(messages_in, context, std::move(new_socket)));
    connections.back()->StartListening();

    acceptor.async_accept(context, BindMember(this, &TCPServer::HandleAccept));
}