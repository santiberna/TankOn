#pragma once

#include <network/udp/multicast.hpp>
#include <network/utility/context_thread.hpp>
#include <network/utility/tick_timer.hpp>
#include <game/data/messages.hpp>
#include <map>

constexpr auto DISCOVER_MESSAGE_TIMER = std::chrono::milliseconds(32);
constexpr auto DISCOVER_CHANNEL = "224.0.0.2";
constexpr uint16_t DISCOVER_PORT = 32102;
constexpr uint16_t GAME_PORT = 16000;

class LobbyBroadcaster
{
public:
    LobbyBroadcaster()
        : sender(context.Context(), DISCOVER_CHANNEL, DISCOVER_PORT)
        , send_timer(context.Context(), DISCOVER_MESSAGE_TIMER, [this]()
              { BroadcastLobbyInfo(); })
    {
        info.hostname = asio::ip::host_name();
        info.ip = sender.GetBroadcastIP().to_string();

        context.Start();
    }

    ~LobbyBroadcaster()
    {
        context.Stop();
    }

    void UpdateLobbyInfo(uint32_t current_player_count, uint32_t max_player_count)
    {
        std::scoped_lock<std::mutex> lock { info_mutex };
        info.current_players = current_player_count;
        info.max_players = max_player_count;
    }

private:
    void BroadcastLobbyInfo()
    {
        std::scoped_lock<std::mutex> lock { info_mutex };
        std::stringstream stream {};
        auto as_string = SerializeStruct(stream, info);
        sender.Send(as_string);
    }

    ContextThread context {};
    UDPMulticastSender sender;
    TickTimer send_timer;

    std::mutex info_mutex {};
    LobbyDiscoverInfo info {};
};

class LobbyDiscoverer
{
public:
    LobbyDiscoverer()
        : listener(context.Context(), DISCOVER_CHANNEL, DISCOVER_PORT)
        , update_timer(context.Context(), DISCOVER_MESSAGE_TIMER, [this]()
              { Update(); })
    {
        listener.QueueStart();
        context.Start();
    }

    ~LobbyDiscoverer()
    {
        context.Stop();
    }

    struct LobbyTrack
    {
        LobbyDiscoverInfo info {};
        std::chrono::milliseconds timestamp {};
    };

    std::vector<LobbyDiscoverInfo> GetFoundLobbies() const
    {
        std::scoped_lock<std::mutex> lock { data_mutex };
        std::vector<LobbyDiscoverInfo> out {};

        for (auto& elem : lobbies)
        {
            out.emplace_back(elem.second.info);
        }

        return out;
    }

    void Update()
    {
        std::scoped_lock<std::mutex> lock { data_mutex };
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

        while (!listener.GetMessages().empty())
        {
            auto msg = listener.GetMessages().pop_front();

            std::stringstream stream { msg };
            auto lobby_info = DeserializeStruct<LobbyDiscoverInfo>(stream);

            lobbies[lobby_info.hostname] = { lobby_info, now };
        }

        std::vector<std::string> to_forget {};
        for (auto& [name, info] : lobbies)
        {
            if (now - info.timestamp > 10 * DISCOVER_MESSAGE_TIMER)
                to_forget.emplace_back(name);
        }

        for (auto& name : to_forget)
            lobbies.erase(name);
    }

private:
    ContextThread context {};
    UDPMulticastListener listener;

    TickTimer update_timer;

    mutable std::mutex data_mutex {};
    std::map<std::string, LobbyTrack> lobbies {};
};