#pragma once
#include <game/gameplay/message_types.hpp>
#include <network/base_classes/client.hpp>
#include <game/gameplay/player.hpp>

class GameClient : public TCPClient
{
public:
    GameClient(const std::string& hostname, uint16_t port)
        : TCPClient(hostname, port)
        , hostname(hostname)
    {
    }

    ~GameClient() override = default;

    uint32_t GetPlayerId() const { return info.current_player; }
    uint32_t GetPlayerCount() const { return info.players.size(); }

    const char* GetHostname() const { return hostname.c_str(); }

    uint32_t GetCurrentPlayerID() const { return info.current_player; }
    PlayerData& GetCurrentPlayer() { return info.players[info.current_player]; }
    const PlayerGlobalTable& GetAllPlayers() const { return info; }
    bool InGame() const { return has_started; }

protected:
    void ProcessMessage(TCPConnection& sender, const Message& message) override;

private:
    PlayerGlobalTable info {};
    std::string hostname {};
    bool has_started {};
};