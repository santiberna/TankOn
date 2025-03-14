#pragma once

#include <network/network_common.hpp>
#include <network/utility/ts_deque.hpp>
#include <utility/log.hpp>

constexpr size_t MAX_MESSAGE_SIZE = 256;

// Sync sender (UDP sending is almost never blocking)
class UDPMulticastSender
{
public:
    UDPMulticastSender(asio::io_context& context, const std::string& multicast_channel, uint16_t port)
        : socket(context, boost::asio::ip::udp::v4())
    {
        socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket.set_option(boost::asio::ip::multicast::enable_loopback(true));

        socket.connect(asio::ip::udp::endpoint(asio::ip::make_address_v4(multicast_channel), port));
    }

    ~UDPMulticastSender()
    {
        if (socket.is_open())
            socket.close();
    }

    asio::ip::address GetBroadcastIP() const
    {
        return socket.local_endpoint().address();
    }

    void Send(const std::string& data)
    {
        assert(data.size() < MAX_MESSAGE_SIZE);

        boost::system::error_code unused {};
        socket.send(asio::buffer(data), 0, unused);
    }

private:
    asio::ip::udp::socket socket;
};

class UDPMulticastListener
{
public:
    UDPMulticastListener(asio::io_context& context, const std::string& multicast_channel, uint16_t port)
        : socket(context, boost::asio::ip::udp::v4())
    {
        socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket.set_option(boost::asio::ip::multicast::enable_loopback(true));

        socket.bind(asio::ip::udp::endpoint(asio::ip::address_v4::any(), port));
        socket.set_option(asio::ip::multicast::join_group(asio::ip::make_address_v4(multicast_channel)));

        message_buf.resize(MAX_MESSAGE_SIZE);
    }

    void QueueStart()
    {
        socket.async_receive(asio::buffer(message_buf.data(), message_buf.size()), [this](std::error_code e, size_t size)
            { HandleRead(e, size); });
    }

    TSDeque<std::string>& GetMessages() { return received; }

private:
    void HandleRead(std::error_code e, size_t size)
    {
        if (e)
        {
            return;
        }

        received.push_back(message_buf.substr(0, size));
        socket.async_receive(asio::buffer(message_buf), [this](std::error_code e, size_t size)
            { HandleRead(e, size); });
    }

    asio::ip::udp::socket socket;

    std::string message_buf {};
    TSDeque<std::string> received {};
};