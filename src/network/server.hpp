#include <network/message.hpp>
#include <network/ts_deque.hpp>
#include <network/connection.hpp>
#include <utility/log.hpp>

class TCPServer
{
public:
    TCPServer(uint16_t port)
        : acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
        acceptor.async_accept(context, BindMember(this, &TCPServer::HandleAccept));
        context_thread = std::thread([this]()
            { context.run(); });
    }

    ~TCPServer()
    {
        context.stop();

        if (context_thread.joinable())
            context_thread.join();
    }

    void WaitAndRespondMessages(std::function<void(TCPConnection& connection, const Message&)> message_handler)
    {
        messages_in.wait_for_entries();

        while (!messages_in.empty())
        {
            auto msg = messages_in.pop_front();
            message_handler(*msg.sender, msg.message);
        }
    }

private:
    void HandleAccept(std::error_code ec, asio::ip::tcp::socket new_socket);

private:
    asio::io_context context {};
    std::thread context_thread {};

    asio::ip::tcp::acceptor acceptor;
    std::vector<std::shared_ptr<TCPConnection>> connections {};

    TSDeque<OwnedMessage> messages_in {};
};
