#pragma once
#include <network/network_common.hpp>

class TCPResolver
{
    using ResolverResult = boost::asio::ip::tcp::resolver::results_type;

public:
    using ResolvedCallback = std::function<void(asio::ip::tcp::socket&&, asio::ip::tcp::endpoint)>;
    using FailedCallback = std::function<void(std::error_code)>;

    TCPResolver(asio::io_context& context, ResolvedCallback on_success, FailedCallback on_fail)
        : resolver(context)
        , socket(context)
        , on_success(on_success)
        , on_fail(on_fail)
    {
    }

    ~TCPResolver()
    {
        resolver.cancel();
    }

    void AttemptConnection(const std::string& hostname, uint16_t port)
    {
        requested_hostname = hostname;
        requested_service = std::to_string(port);

        resolver.async_resolve(requested_hostname, requested_service,
            [this](std::error_code ec, ResolverResult result)
            {
                HandleResolve(ec, std::move(result));
            });
    }

private:
    void HandleResolve(std::error_code ec, ResolverResult&& result)
    {
        requested_hostname.clear();
        requested_service.clear();

        if (ec)
        {
            if (on_fail)
                on_fail(ec);
            return;
        }

        asio::async_connect(socket, std::move(result),
            [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
            {
                HandleConnection(ec, endpoint);
            });
    }

    void HandleConnection(std::error_code ec, asio::ip::tcp::endpoint endpoint)
    {
        if (ec)
        {
            if (on_fail)
                on_fail(ec);
            return;
        }

        if (on_success)
        {
            on_success(std::move(socket), endpoint);
            socket = asio::ip::tcp::socket { resolver.get_executor() };
        }
    }

    std::string requested_hostname {};
    std::string requested_service {};

    asio::ip::tcp::resolver resolver;
    asio::ip::tcp::socket socket;

    ResolvedCallback on_success {};
    FailedCallback on_fail {};
};