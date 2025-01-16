#include <network/base_classes/server.hpp>

TCPServer::TCPServer(uint16_t port)
    : acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
    acceptor.async_accept(context, BindMember(this, &TCPServer::HandleAccept));
    context_thread = std::thread([this]()
        { context.run(); });
}

TCPServer::~TCPServer()
{
    context.stop();

    if (context_thread.joinable())
        context_thread.join();
}

void TCPServer::Run()
{
    while (!context.stopped())
    {
        messages_in.wait_for_entries();

        while (!messages_in.empty())
        {
            std::scoped_lock<std::mutex> lock { context_mutex };
            auto msg = messages_in.pop_front();
            ProcessMessage(*msg.sender, msg.message);
        }
    }
}

void TCPServer::HandleAccept(std::error_code ec, asio::ip::tcp::socket new_socket)
{
    if (ec)
    {
        Log("Failed to accept connection: {}", ec.message());
        return;
    }

    auto connection = std::make_shared<TCPConnection>(messages_in, context, std::move(new_socket));

    if (OnAcceptConnection(*connection))
    {
        std::scoped_lock<std::mutex> lock { context_mutex };
        connections.emplace_back(connection);
        connections.back()->SetId(connections.size() - 1);
        connections.back()->StartListening();
    }

    acceptor.async_accept(context, BindMember(this, &TCPServer::HandleAccept));
}

void TCPServer::Broadcast(const Message& msg, TCPConnection* ignore)
{
    for (auto& connection : connections)
    {
        if (connection.get() == ignore)
            continue;

        Send(*connection, msg);
    }
}

void TCPServer::Send(TCPConnection& connection, const Message& msg)
{
    if (connection.GetSocket().is_open())
    {
        connection.Post(msg);
    }
    else
    {
        RemoveConnection(connection);
    }
}

void TCPServer::RemoveConnection(TCPConnection& connection)
{
    OnDisconnection(connection);

    auto it = std::find_if(connections.begin(), connections.end(), [&connection](const auto& other)
        { return other.get() == &connection; });

    if (it != connections.end())
    {
        std::scoped_lock<std::mutex> lock { context_mutex };
        connections.erase(it);
    }
}