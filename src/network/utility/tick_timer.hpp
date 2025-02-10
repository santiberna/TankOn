#pragma once
#include <utility/timer.hpp>
#include <network/network_common.hpp>
#include <functional>

class TickTimer
{
public:
    using Event = std::function<void()>;

    TickTimer(asio::io_context& context, std::chrono::milliseconds time, Event ev)
        : timer(context, time)
        , ev(ev)
    {
        timer.async_wait([this](std::error_code e)
            { Handler(e); });
    }

    void SetTimerExpire(std::chrono::milliseconds time)
    {
        timer.expires_after(time);
        timer.async_wait([this](std::error_code e)
            { Handler(e); });
    }

    void Cancel()
    {
        timer.cancel();
    }

private:
    void Handler(std::error_code e)
    {
        if (e)
        {
            return;
        }

        timer.async_wait([this](std::error_code e)
            { Handler(e); });

        if (ev)
        {
            ev();
        }
    }

    asio::steady_timer timer;
    Event ev;
};