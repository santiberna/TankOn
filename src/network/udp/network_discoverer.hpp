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
        socket.open(asio::ip::udp::v4());

        out_buf = "Hello World!";
        boost::system::error_code e;
        socket.send_to(asio::buffer(out_buf), receiver_endpoint, {}, e);

        if (e)
        {
            std::cout << e.message() << std::endl;
        }

        in_buf.resize(128);
        auto len = socket.receive_from(asio::buffer(in_buf), sender_endpoint, {}, e);

        if (e)
        {
            std::cout << e.message() << std::endl;
        }

        std::cout.write(in_buf.data(), len);
    }

private:
    std::string in_buf {};
    std::string out_buf {};

    asio::ip::udp::endpoint sender_endpoint;
};