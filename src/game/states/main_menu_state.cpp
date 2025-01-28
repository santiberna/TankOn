#include <game/states/game_states.hpp>
#include <utility/imgui_common.hpp>
#include <game/game.hpp>

void MainMenuState::Update(Game& game, DeltaMS deltatime)
{
    game.renderer.ClearScreen({ 0.0f, 0.0f, 0.0f });

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
    game.server = std::make_unique<GameServer>(cached_startup_info);
    game.client = std::make_unique<GameClient>(cached_ip, SERVER_DEFAULT_PORT);

    if (game.client->IsConnected())
    {
        game.SetGameState(GameStates::LOBBY);
    }
    else
    {
        Log("Failed to initialize Server!");
    }
}

void MainMenuState::AttemptJoinServer(Game& game)
{
    game.client = std::make_unique<GameClient>(cached_ip, SERVER_DEFAULT_PORT);

    if (game.client->IsConnected())
    {
        game.SetGameState(GameStates::LOBBY);
    }
    else
    {
        Log("Failed to join Server!");
    }
}