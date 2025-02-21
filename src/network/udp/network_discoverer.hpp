#include <network/network_common.hpp>
#include <network/utility/tick_timer.hpp>

#include <iostream>

class NetworkDiscoverer
{
public:
    using MessageHandler = std::function<void(const std::string& message)>;

    NetworkDiscoverer(asio::io_context& context, uint16_t listener_port, MessageHandler handler)
    {
        asio::ip::udp::resolver resolver { context };
        asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(asio::ip::udp::v4(), "localhost", std::to_string(listener_port)).begin();

        asio::ip::udp::socket socket { context };

        socket.async_receive_from(in_buf, sender_endpoint, [this](std::error_code e, size_t len)
            { std::cout.write(in_buf.data(), len); });

        out_buf = "Hello World!";
        socket.send_to(out_buf, receiver_endpoint);
    }

private:
    std::string in_buf {};
    std::string out_buf {};

    asio::ip::udp::endpoint sender_endpoint;
};