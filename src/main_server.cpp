#include <network/server.hpp>

enum MessageType
{
    PING = 0,
};

void ServerMessageHandler(TCPConnection& sender, const Message& msg)
{
    switch (msg.header.message_type)
    {
    case MessageType::PING:
        sender.Post({ MessageType::PING, msg.body.data });
        break;
    default:
        break;
    }
}

int main(int argc, char* args[])
{
    try
    {
        TCPServer server { 6000 };

        while (true)
        {
            Log("Waiting for Messages");
            server.WaitAndRespondMessages(ServerMessageHandler);
        }
    }
    catch (const std::exception& e)
    {
        Log("Network Error: {}", e.what());
    }
}