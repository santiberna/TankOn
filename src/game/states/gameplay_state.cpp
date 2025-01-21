#include <game/states/gameplay_state.hpp>
#include <game/states/main_menu_state.hpp>
#include <game/game.hpp>
#include <utility/imgui_common.hpp>

GameplayState::GameplayState(std::unique_ptr<GameServer>&& server, std::unique_ptr<GameClient>&& client)
    : is_host(server != nullptr)
    , server(std::move(server))
    , client(std::move(client))
{
    camera.translation = { 1600 * 0.5f, 900 * 0.5f };
    camera.scale = { 0.4f, 0.4f };
}

void GameplayState::ExecuteFrame(Game& game, DeltaMS deltatime)
{
    if (client->IsConnected())
    {
        client->ProcessMessages();
    }
    else
    {
        ExitGame(game);
        return;
    }

    auto& current_player = client->GetCurrentPlayer();
    game.UpdatePlayer(current_player, camera, deltatime);

    client->Send(AsMessage(MessageType::eUPDATE_PLAYER, std::make_pair(client->GetCurrentPlayerID(), current_player)));

    game.renderer.ClearScreen({ 0.8f, 0.7f, 0.5f });

    auto& world = client->GetAllPlayers();
    for (auto&& [id, player] : world.players)
    {
        game.DrawPlayer(camera, player, id - 1);
    }
}

void GameplayState::ExitGame(Game& game)
{
    client.reset();
    server.reset();

    game.SetGameState(new MainMenuState());
}