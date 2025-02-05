#pragma once
#include <network/network_common.hpp>

class ContextThread
{
public:
    ContextThread() = default;
    ~ContextThread() { Stop(); }

    void Start()
    {
        thread = std::thread { [this]()
            { context.run(); } };
    }

    void Stop()
    {
        if (!context.stopped())
        {
            context.stop();
        }

        if (thread.joinable())
            thread.join();
    }

    asio::io_context& Context() { return context; }

private:
    asio::io_context context;
    std::thread thread;
};