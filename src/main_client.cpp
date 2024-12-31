#include <network/client.hpp>

enum MessageType
{
    PING = 0,
};

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

int main(int argc, char* args[])
{
    using namespace std::chrono_literals;

    std::string ip_address;
    std::cout << "Provide IP Address to connect: ";
    std::cin >> ip_address;
    std::cout << std::endl;

    try
    {
        TCPClient client { ip_address, 6000 };

        if (!client.IsConnected())
            return 1;

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