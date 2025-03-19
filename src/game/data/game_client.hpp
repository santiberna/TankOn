#pragma once
#include <utility/log.hpp>
#include <network/utility/context_thread.hpp>
#include <network/tcp/connection.hpp>
#include <network/tcp/resolver.hpp>

#include <game/data/messages.hpp>
#include <utility/timer.hpp>
#include <game/data/constants.hpp>
#include <network/utility/tick_timer.hpp>

class Application;

enum class ConnectionResult
{
    WAITING,
    SUCCESS,
    FAIL
};

class GameClient
{
public:
    GameClient(const std::string& username, const std::string& address, uint16_t port);
    ~GameClient() { context_thread.Stop(); }

    void ProcessMessages(Application& application);
    ConnectionResult GetConnectionResult() const { return result.load(); }

    LobbyInfo GetLobbyInfo() const
    {
        std::scoped_lock<std::mutex> lock { client_mutex };
        return lobby_state;
    }

    WorldInfo GetWorldInfo() const
    {
        std::scoped_lock<std::mutex> lock { client_mutex };
        return world_state;
    }

    uint32_t GetPlayerIndex() const
    {
        std::scoped_lock<std::mutex> lock { client_mutex };
        return player_id;
    }

    TCPConnection& GetConnection()
    {
        return *connection;
    }

    void UpdateControlledPlayer(const PlayerInfo& data)
    {
        std::scoped_lock<std::mutex> lock { client_mutex };
        world_state.players.at(player_id) = data;
        connection->Post(AsMessage(BOTH_UPDATE_PLAYER, PlayerUpdate { player_id, data }));
    }

    void ShootBullet(const BulletInfo& bullet)
    {
        std::scoped_lock<std::mutex> lock { client_mutex };
        connection->Post(AsMessage(CLIENT_SHOOT_BULLET, bullet));
    }

    void ClearDeadBullets()
    {
        auto now = GetEpochMS();

        {
            std::scoped_lock<std::mutex> lock { client_mutex };

            std::erase_if(world_state.bullets,
                [now](const BulletInfo& i)
                { return i.start_time + BULLET_LIFETIME_MS < now.count(); });
        }
    }

    std::optional<uint32_t> GetGameWinner() const
    {
        std::scoped_lock<std::mutex> lock { client_mutex };
        return game_winner;
    }

    uint32_t GetPingMS() const
    {
        std::scoped_lock<std::mutex> lock { client_mutex };
        return ping_ms;
    }

private:
    ContextThread context_thread;
    mutable std::mutex client_mutex {};

    std::atomic<ConnectionResult> result { ConnectionResult::WAITING };
    std::unique_ptr<TCPResolver> resolver;
    std::unique_ptr<TCPConnection> connection;

    std::optional<uint32_t> game_winner {};

    std::string player_name {};
    uint32_t player_id = -1;

    LobbyInfo lobby_state;
    WorldInfo world_state;

    // DEBUG
    std::unique_ptr<TickTimer> ping_messager {};
    uint32_t ping_ms {};
};
