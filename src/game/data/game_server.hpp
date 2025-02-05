#pragma once
#include <utility/log.hpp>
#include <network/utility/context_thread.hpp>
#include <network/utility/tick_timer.hpp>
#include <network/tcp/connection.hpp>
#include <network/tcp/acceptor.hpp>
#include <game/data/messages.hpp>
#include <game/data/constants.hpp>

struct GameConnection
{
    std::unique_ptr<TCPConnection> connection;
    UserData user_data {};
};

class GameServer
{
public:
    GameServer(uint16_t port, uint32_t max_players);
    ~GameServer() { context_thread.Stop(); }

private:
    void Broadcast(const Message& msg, const TCPConnection* ignore);
    void HandleDisconnects();
    void HandleConnection(asio::ip::tcp::socket&& socket);
    void ProcessMessages();

    void CheckBulletCollisions();

    ContextThread context_thread;
    std::unique_ptr<TCPAcceptor> acceptor;
    std::unique_ptr<TickTimer> timer;

    // Lobby
    uint32_t max_players;
    std::vector<GameConnection> lobby_connections;

    // Game
    bool started = false;
    uint32_t bullet_id_gen = 0;
    WorldInfo world_info {};
};