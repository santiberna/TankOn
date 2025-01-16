#pragma once
#include <network/base_classes/server.hpp>
#include <game/gameplay/message_types.hpp>
#include <game/gameplay/player.hpp>

constexpr auto SERVER_DEFAULT_PORT = 6000;

struct GameInitInfo
{
    uint32_t numPlayers = 2;
};

class GameServer : public TCPServer
{
public:
    GameServer(const GameInitInfo& info = {});
    ~GameServer() override;

protected:
    void ProcessMessage(TCPConnection& sender, const Message& message) override;

    bool OnAcceptConnection(TCPConnection& sender) override;
    void OnDisconnection(TCPConnection& connection) override;
    void CheckLobbyConnections(std::error_code e);

    void StartGame();

private:
    PlayerGlobalTable player_data {};

    bool ingame = false;

    std::unique_ptr<asio::steady_timer> lobby_check_timer;
    std::thread server_thread;
};