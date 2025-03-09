#pragma once

#include <network/udp/multicast.hpp>
#include <network/utility/context_thread.hpp>
#include <game/data/messages.hpp>

class LobbyDiscoverer
{
public:
    static constexpr auto DISCOVER_CHANNEL = "224.0.0.2";
    static constexpr uint16_t DISCOVER_PORT = 32102;

    LobbyDiscoverer()
        : listener(context.Context(), DISCOVER_CHANNEL, DISCOVER_PORT)
    {
        context.Start();
    }

    std::vector<LobbyDiscoverInfo> FindLobbies()
    {
        std::vector<LobbyDiscoverInfo> out {};
        while (!listener.GetMessages().empty())
        {
            auto msg = listener.GetMessages().pop_front();
            auto lobby_info = DeserializeStruct<LobbyDiscoverInfo>(msg);

            out.emplace_back(std::move(lobby_info));
        }

        return out;
    }

private:
    ContextThread context {};
    UDPMulticastListener listener;
};