#include <game/states/game_states.hpp>
#include <game/game.hpp>
#include <utility/imgui_common.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

GameplayState::GameplayState()
{
    camera.translation = { 1600 * 0.5f, 900 * 0.5f };
    camera.scale = { 0.4f, 0.4f };
}

void GameplayState::Update(Game& game, DeltaMS deltatime)
{
    if (game.client->IsConnected())
    {
        game.client->ProcessMessages();
    }
    else
    {
        ExitGame(game);
        return;
    }

    auto& current_player = game.client->GetCurrentPlayer();

    UpdatePlayer(game, current_player, deltatime);

    game.client->Send(AsMessage(MessageType::eUPDATE_PLAYER,
        std::make_pair(game.client->GetCurrentPlayerID(), current_player)));

    game.renderer.ClearScreen({ 0.8f, 0.7f, 0.5f });

    auto& world = game.client->GetAllPlayers();
    for (auto&& [id, player] : world.players)
    {
        game.DrawPlayer(camera, player, id - 1);
    }
}

void GameplayState::UpdatePlayer(Game& game, PlayerData& out_data, DeltaMS deltatime)
{
    glm::vec2 movement {};

    if (game.input.GetKey(SDLK_W))
    {
        movement += UP;
    }
    if (game.input.GetKey(SDLK_S))
    {
        movement -= UP;
    }
    if (game.input.GetKey(SDLK_D))
    {
        movement += RIGHT;
    }
    if (game.input.GetKey(SDLK_A))
    {
        movement -= RIGHT;
    }

    if (glm::epsilonNotEqual(glm::length(movement), 0.0f, glm::epsilon<float>()))
    {
        movement = glm::normalize(movement);
        out_data.velocity = movement * deltatime.count();
        out_data.position += out_data.velocity;
    }

    glm::vec2 player_screen = camera * out_data.position;
    glm::vec2 tank_to_cursor = glm::normalize(game.input.GetMousePos() - player_screen);

    out_data.rotation = -glm::degrees(VectorAngle(UP, tank_to_cursor));
}

void GameplayState::ExitGame(Game& game)
{
    game.client.reset();
    game.server.reset();

    game.SetGameState(GameStates::MAIN_MENU);
}