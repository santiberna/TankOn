#include <SDL3/SDL_main.h>
#include <Game.hpp>
#include <utility/log.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type-mismatch"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <boost/asio.hpp>
#pragma clang diagnostic pop

namespace ip = boost::asio::ip;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
public:
    static std::shared_ptr<TCPConnection> Create(boost::asio::io_context& io_context)
    {
        return std::shared_ptr<TCPConnection> { new TCPConnection(io_context) };
    }

    ip::tcp::socket& GetSocket()
    {
        return socket;
    }

    void Start()
    {
        message = "Hello World!";

        boost::asio::async_write(socket, boost::asio::buffer(message),
            std::bind(&TCPConnection::HandleWrite, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

private:
    TCPConnection(boost::asio::io_context& io_context)
        : socket(io_context)
    {
    }

    void HandleWrite(const boost::system::error_code& /*error*/,
        size_t /*bytes_transferred*/)
    {
    }

    std::string message {};
    ip::tcp::socket socket;
};

class TCPServer
{
public:
    TCPServer(boost::asio::io_context& io_context)
        : io_context(io_context)
        , acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), 13))
    {
        StartAccept();
    }

    void StartAccept()
    {
        auto new_connection = TCPConnection::Create(io_context);

        Log("Accepting a connection!");
        acceptor.async_accept(new_connection->GetSocket(),
            std::bind(&TCPServer::HandleAccept, this, new_connection, boost::asio::placeholders::error));
    }

    void HandleAccept(std::shared_ptr<TCPConnection> new_connection, const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->Start();
        }

        StartAccept();
    }

private:
    boost::asio::io_context& io_context;
    ip::tcp::acceptor acceptor;
};

void server_entry()
{
    boost::asio::io_context io_context {};

    try
    {
        boost::asio::io_context io_context;
        TCPServer server(io_context);
        io_context.run();
    }
    catch (const std::exception& e)
    {
        Log("Network Error: {}", e.what());
    }
}

void client_entry()
{
    boost::asio::io_context io_context {};
    ip::tcp::resolver resolver(io_context);

    try
    {
        auto endpoints = resolver.resolve("localhost", std::to_string(13));

        ip::tcp::socket socket(io_context);

        Log("Attempting to connect");
        boost::asio::connect(socket, endpoints);
        Log("Connection succeded");

        boost::system::error_code error;

        std::array<char, 256> buffer {};
        auto s = socket.read_some(boost::asio::buffer(buffer), error);
        Log("Bytes Received: {}", s);

        if (error == boost::asio::error::eof)
            return;
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        Log("{}", buffer.data());
    }
    catch (const std::exception& e)
    {
        Log("Network Error: {}", e.what());
    }

    std::cin.get();
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

// int main(int argc, char* args[])
// {
//     namespace ip = boost::asio::ip;

//     boost::asio::io_context io_context;

//     std::string start_config {};
//     std::cin >> start_config;

//     std::string address = "localhost";
//     uint32_t port = 42;

//     try
//     {
//         if (start_config == "server")
//         {
//             Log("Started Server");

//             ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), port));
//             ip::tcp::socket socket(io_context);
//             acceptor.accept(socket);

//             for (;;)
//             {
//                 std::string message = "Hello World!";
//                 boost::system::error_code ignored_error;
//                 boost::asio::write(socket, boost::asio::buffer(message), ignored_error);

//                 try
//                 {
//                     size_t bytes_transferred = boost::asio::write(socket, boost::asio::buffer(message));
//                     std::cout << "Message sent, bytes transferred: " << bytes_transferred << "\n";
//                 }
//                 catch (const boost::system::system_error& e)
//                 {
//                     std::cerr << "Error sending message: " << e.what() << "\n";
//                     break;
//                 }

//                 std::this_thread::sleep_for(std::chrono::milliseconds(100));
//             }
//         }
//         else if (start_config == "client")
//         {
//             Log("Started Client");
//             ip::tcp::resolver resolver(io_context);

//             auto endpoints = resolver.resolve(address, std::to_string(port));

//             ip::tcp::socket socket(io_context);
//             boost::asio::connect(socket, endpoints);

//             for (;;)
//             {
//                 std::array<char, 128> buf;
//                 boost::system::error_code error;

//                 size_t len = socket.read_some(boost::asio::buffer(buf), error);
//                 if (error == boost::asio::error::eof)
//                     break; // Connection closed cleanly by peer.
//                 else if (error)
//                     throw boost::system::system_error(error); // Some other error.

//                 std::cout.write(buf.data(), len);
//             }
//         }
//     }
//     catch (const std::exception& e)
//     {
//         Log("Error: {}", e.what());
//     }

//     return 0;
// }
