#include <network/containers/message.hpp>
#include <network/containers/ts_deque.hpp>
#include <network/connection/connection.hpp>
#include <utility/log.hpp>

class TCPServer
{
public:
    TCPServer(uint16_t port);
    virtual ~TCPServer();

    void Run();

protected:
    virtual void ProcessMessage(
        [[maybe_unused]] TCPConnection& sender,
        [[maybe_unused]] const Message& message)
    {
    }

    virtual bool OnAcceptConnection(
        [[maybe_unused]] TCPConnection& connection)
    {
        return true;
    }

    virtual void OnDisconnection(
        [[maybe_unused]] TCPConnection& connection)
    {
    }

    void NotifyServer()
    {
        messages_in.push_front({});
    };

    void Send(TCPConnection& connection, const Message& msg);
    void Broadcast(const Message& msg, TCPConnection* ignore = nullptr);
    void RemoveConnection(TCPConnection& connection);
    void Stop() { context.stop(); }

    asio::io_context context {};
    std::vector<std::shared_ptr<TCPConnection>> connections {};

private:
    void HandleAccept(std::error_code ec, asio::ip::tcp::socket new_socket);

    std::mutex context_mutex {};
    std::thread context_thread {};
    asio::ip::tcp::acceptor acceptor;
    TSDeque<OwnedMessage> messages_in {};
};
