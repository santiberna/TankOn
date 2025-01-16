#pragma once
#include <game/states/base_state.hpp>
#include <game/gameplay/game_server.hpp>
#include <game/gameplay/game_client.hpp>

class LobbyState : public BaseGameState
{
public:
    LobbyState(std::unique_ptr<GameServer>&& server, std::unique_ptr<GameClient>&& client)
        : is_host(true)
        , server(std::move(server))
        , client(std::move(client))
    {
    }

    LobbyState(std::unique_ptr<GameClient>&& client)
        : is_host(false)
        , client(std::move(client))
    {
    }

    ~LobbyState() override = default;

    void StartGamemode(Game& game);
    void ExecuteFrame(Game& game, DeltaMS deltatime) override;
    void ExitLobby(Game& game);

private:
    bool is_host {};
    std::unique_ptr<GameServer> server {};
    std::unique_ptr<GameClient> client {};
};