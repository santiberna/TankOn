#pragma once

#include <network/udp/multicast.hpp>
#include <network/utility/context_thread.hpp>
#include <network/utility/tick_timer.hpp>
#include <game/data/messages.hpp>

class LobbyBroadcaster
{
    static constexpr auto DISCOVER_MESSAGE_TIMER = std::chrono::milliseconds(32);
    static constexpr auto DISCOVER_CHANNEL = "224.0.0.2";
    static constexpr uint16_t DISCOVER_PORT = 32102;

    LobbyBroadcaster()
        : sender(context.Context(), DISCOVER_CHANNEL, DISCOVER_PORT)
        , send_timer(context.Context(), DISCOVER_MESSAGE_TIMER, [this]()
              { BroadcastLobbyInfo(); })
    {
        context.Start();
    }

    void UpdateLobbyInfo(const LobbyDiscoverInfo& lobby)
    {
        std::scoped_lock<std::mutex> lock { info_mutex };
        info = lobby;
    }

private:
    void BroadcastLobbyInfo()
    {
        std::scoped_lock<std::mutex> lock { info_mutex };
        auto as_string = SerializeStruct(info);
        sender.Send(as_string);
    }

    ContextThread context {};
    UDPMulticastSender sender;
    TickTimer send_timer;

    std::mutex info_mutex {};
    LobbyDiscoverInfo info {};
};