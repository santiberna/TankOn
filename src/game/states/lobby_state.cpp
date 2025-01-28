#include <game/states/game_states.hpp>
#include <utility/imgui_common.hpp>
#include <game/game.hpp>

void LobbyState::Update(Game& game, DeltaMS deltatime)
{
    game.renderer.ClearScreen({ 0.0f, 0.0f, 0.0f });

    if (!game.client->IsConnected())
    {
        ExitLobby(game);
        return;
    }
    else
    {
        game.client->ProcessMessages();
    }

    if (game.client->InGame())
    {
        StartGamemode(game);
        return;
    }

    ImGui::Begin("Lobby");
    ImGui::Text("Waiting for Players... (%u/%u)", game.client->GetPlayerCount(), game.client->GetAllPlayers().target_players);
    ImGui::Text("You are player %u.", game.client->GetPlayerId());
    ImGui::Separator();

    if (game.server)
    {
        ImGui::Text("You are currently the host");

        if (ImGui::Button("Close Server"))
        {
            ExitLobby(game);
        }
    }
    else
    {
        ImGui::Text("You are currently connected to %s", game.client->GetHostname());

        if (ImGui::Button("Exit Server"))
            ExitLobby(game);
    }

    ImGui::End();
}

void LobbyState::StartGamemode(Game& game)
{
    game.SetGameState(GameStates::GAME);
}

void LobbyState::ExitLobby(Game& game)
{
    game.client.reset();
    game.server.reset();

    game.SetGameState(GameStates::MAIN_MENU);
}