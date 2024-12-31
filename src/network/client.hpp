#include <string>
#include <thread>
#include <network/connection.hpp>

class TCPClient
{
public:
    TCPClient(const std::string& hostname, uint16_t port);

    ~TCPClient()
    {
        Disconnect();
    }

    bool IsConnected()
    {
        return connection && connection->GetSocket().is_open();
    }

    void Send(const Message& msg)
    {
        if (connection)
            connection->Post(msg);
    }

    void Disconnect()
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

    void ProcessMessages(std::function<void(TCPConnection& connection, const Message&)> message_handler)
    {
        while (!messages_in.empty())
        {
            auto msg = messages_in.pop_front();
            message_handler(*msg.sender, msg.message);
        }
    }

    void WaitForMessage()
    {
        messages_in.wait_for_entries();
    }

private:
    TSDeque<OwnedMessage> messages_in {};
    asio::io_context context {};
    std::thread context_thread {};

    std::shared_ptr<TCPConnection> connection {};
};