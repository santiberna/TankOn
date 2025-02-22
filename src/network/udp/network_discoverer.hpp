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

// void server()
// {
//     using namespace asio::ip;

//     try
//     {
//         boost::asio::io_context io_context;
//         udp::socket socket(io_context, udp::endpoint(udp::v4(), 6000));

//         for (;;)
//         {
//             std::array<char, 1> recv_buf;
//             udp::endpoint remote_endpoint {};
//             socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

//             std::string message = "Hello World!";
//             boost::system::error_code ignored_error;
//             socket.send_to(boost::asio::buffer(message),
//                 remote_endpoint, 0, ignored_error);
//         }
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
// }

// void client()
// {
//     using namespace asio::ip;

//     try
//     {
//         asio::io_context io_context;
//         asio::ip::udp::resolver resolver(io_context);
//         asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "localhost", "6000").begin();

//         udp::socket socket(io_context);
//         socket.open(udp::v4());

//         std::array<char, 1> send_buf = { { 0 } };
//         socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

//         std::array<char, 128> recv_buf;
//         udp::endpoint sender_endpoint;
//         size_t len = socket.receive_from(
//             boost::asio::buffer(recv_buf), sender_endpoint);

//         std::cout.write(recv_buf.data(), len);
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
// }
