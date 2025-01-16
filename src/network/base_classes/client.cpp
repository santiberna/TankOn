#include <network/base_classes/client.hpp>

TCPClient::TCPClient(const std::string& hostname, uint16_t port)
{
    asio::ip::tcp::resolver resolver { context };
    connection = std::make_shared<TCPConnection>(messages_in, context, asio::ip::tcp::socket(context));

    auto endpoints = resolver.resolve(hostname, std::to_string(port));
    auto ip = asio::connect(connection->GetSocket(), endpoints);

    Log("Connected to {}", ip.address().to_string());
    connection->StartListening();

    context_thread = std::thread([this]()
        { context.run(); });
}

TCPClient::~TCPClient()
{
    Disconnect();
}

void TCPClient::Disconnect()
{
    if (connection->GetSocket().is_open())
    {
        asio::post(context, [this]()
            { connection->GetSocket().close(); });
    }

    context.stop();

    if (context_thread.joinable())
        context_thread.join();
}