#include <string>
#include <thread>
#include <network/connection/connection.hpp>

class TCPClient
{
public:
    TCPClient(const std::string& hostname, uint16_t port);
    virtual ~TCPClient();

    bool IsConnected()
    {
        return connection && connection->GetSocket().is_open();
    }

    void Send(const Message& msg)
    {
        if (connection)
            connection->Post(msg);
    }

    void ProcessMessages()
    {
        while (!messages_in.empty())
        {
            auto msg = messages_in.pop_front();
            ProcessMessage(*msg.sender, msg.message);
        }
    }

    void WaitForMessage()
    {
        messages_in.wait_for_entries();
    }

protected:
    void Disconnect();

    virtual void ProcessMessage(
        [[maybe_unused]] TCPConnection& sender,
        [[maybe_unused]] const Message& message)
    {
    }

private:
    TSDeque<OwnedMessage> messages_in {};
    asio::io_context context {};
    std::thread context_thread {};

    std::shared_ptr<TCPConnection> connection {};
};