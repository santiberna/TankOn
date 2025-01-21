#include <game/states/lobby_state.hpp>
#include <game/states/main_menu_state.hpp>
#include <utility/imgui_common.hpp>
#include <game/states/gameplay_state.hpp>
#include <game/game.hpp>

void LobbyState::ExecuteFrame(Game& game, DeltaMS deltatime)
{
    game.renderer.ClearScreen({ 0.0f, 0.0f, 0.0f });

    if (!client->IsConnected())
    {
        ExitLobby(game);
        return;
    }
    else
    {
        client->ProcessMessages();
    }

    if (client->InGame())
    {
        StartGamemode(game);
        return;
    }

    ImGui::Begin("Lobby");
    ImGui::Text("Waiting for Players... (%u/%u)", client->GetPlayerCount(), client->GetAllPlayers().target_players);
    ImGui::Text("You are player %u.", client->GetPlayerId());
    ImGui::Separator();

    if (is_host)
    {
        ImGui::Text("You are currently the host");

        if (ImGui::Button("Close Server"))
        {
            ExitLobby(game);
        }
    }
    else
    {
        ImGui::Text("You are currently connected to %s", client->GetHostname());

        if (ImGui::Button("Exit Server"))
            ExitLobby(game);
    }

    ImGui::End();
}

void LobbyState::StartGamemode(Game& game)
{
    game.SetGameState(new GameplayState(std::move(server), std::move(client)));
}

void LobbyState::ExitLobby(Game& game)
{
    client.reset();
    server.reset();

    game.SetGameState(new MainMenuState());
}