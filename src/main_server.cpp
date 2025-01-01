#include <network/server.hpp>

enum MessageType
{
    PING = 0,
    ECHO = 1
};

void ServerMessageHandler(TCPConnection& sender, const Message& msg)
{
    switch (msg.header.message_type)
    {
    case MessageType::PING:
        sender.Post({ MessageType::PING, msg.body.data });
        break;
    case MessageType::ECHO:
    {
        std::string message = msg.body.data;
        std::reverse(message.begin(), message.end());
        sender.Post(Message(MessageType::ECHO, message));
    }
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