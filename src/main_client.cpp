#include <network/client.hpp>

enum MessageType
{
    ePING = 0,
    eECHO = 1
};

void ClientMessageHandler(TCPConnection& sender, const Message& msg)
{
    switch (msg.header.message_type)
    {
    case MessageType::ePING:
    {
        auto now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto then = std::stoll(msg.body.data);
        auto ping_time = now - then;
        Log("Ping took: {} microseconds", ping_time);
        break;
    }
    case MessageType::eECHO:
    {
        Log("Received: {}", msg.body.data);
    }
    }
}

int main(int argc, char* args[])
{
    using namespace std::chrono_literals;

    std::string ip_address;
    std::cout << "Provide IP Address to connect: ";
    std::cin >> ip_address;

    try
    {
        TCPClient client { ip_address, 6000 };

        if (!client.IsConnected())
            return 1;

        while (true)
        {
            // auto now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
            // auto msg = Message(PING, std::to_string(now.count()));

            std::string echo {};
            std::cout << "Send Message: ";
            std::getline(std::cin, echo);

            client.Send(Message(MessageType::eECHO, echo));
            client.WaitForMessage();
            client.ProcessMessages(ClientMessageHandler);
        }
    }
    catch (const std::exception& e)
    {
        Log("Network Error: {}", e.what());
    }
}