#include <game/states/main_menu_state.hpp>
#include <game/states/lobby_state.hpp>
#include <game/gameplay/game_server.hpp>
#include <game/gameplay/game_client.hpp>
#include <utility/imgui_common.hpp>
#include <game/game.hpp>

void MainMenuState::ExecuteFrame(Game& game, DeltaMS deltatime)
{
    game.renderer.ClearScreen(0, 0, 0);

    ImGui::Begin("Main Menu");

    ImGui::Text("IP to connect:");
    ImGui::InputText("IPv4", &cached_ip);

    if (ImGui::Button("Connect to Game"))
    {
        AttemptJoinServer(game);
    }

    ImGui::Separator();

    if (ImGui::Button("Host Game"))
    {
        AttemptStartServer(game);
    }

    int temp = static_cast<int>(cached_startup_info.numPlayers);
    if (ImGui::SliderInt("Number of players", &temp, 1, 4))
        cached_startup_info.numPlayers = static_cast<uint32_t>(temp);

    ImGui::End();
}

void MainMenuState::AttemptStartServer(Game& game)
{
    std::unique_ptr<GameServer> server = std::make_unique<GameServer>(cached_startup_info);
    std::unique_ptr<GameClient> client = std::make_unique<GameClient>(cached_ip, SERVER_DEFAULT_PORT);

    if (client->IsConnected())
    {
        game.SetGameState(new LobbyState(std::move(server), std::move(client)));
    }
    else
    {
        Log("Failed to initialize Server!");
    }
}

void MainMenuState::AttemptJoinServer(Game& game)
{
    std::unique_ptr<GameClient> client = std::make_unique<GameClient>(cached_ip, SERVER_DEFAULT_PORT);

    if (client->IsConnected())
    {
        game.SetGameState(new LobbyState(std::move(client)));
    }
    else
    {
        Log("Failed to join Server!");
    }
}