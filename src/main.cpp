#include <Game.hpp>
#include <SDL3/SDL_main.h>
#include <chrono>

#include <thread>
#include <network/server.hpp>
#include <network/client.hpp>

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

void ClientMessageHandler(TCPConnection& sender, const Message& msg)
{
    switch (msg.header.message_type)
    {
    case MessageType::PING:
    {
        auto now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto then = std::stoll(msg.body.data);
        auto ping_time = now - then;
        Log("Ping took: {}ms", ping_time);
        break;
    }
    }
}

void server_entry()
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

void client_entry()
{
    using namespace std::chrono_literals;

    try
    {
        TCPClient client { "localhost", 6000 };

        if (!client.IsConnected())
            return;

        while (true)
        {
            auto now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
            auto msg = Message(PING, std::to_string(now.count()));

            client.Send(msg);
            client.WaitForMessage();
            client.ProcessMessages(ClientMessageHandler);

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    catch (const std::exception& e)
    {
        Log("Network Error: {}", e.what());
    }
}

int main(int argc, char* args[])
{
    std::string start_config {};
    std::cin >> start_config;

    if (start_config == "server")
    {
        server_entry();
    }
    else if (start_config == "client")
    {
        client_entry();
    }

    return 0;
}