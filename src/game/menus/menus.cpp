#include <game/menus/menus.hpp>
#include <game/application.hpp>

void MainMenu::UpdateMenu(Application& application)
{
    ImGui::Begin("Main Menu");

    ImGui::InputText("Username", &cached_username);
    ImGui::Separator();

    ImGui::Text("IP to connect:");
    ImGui::InputText("IPv4", &cached_ip);

    if (ImGui::Button("Connect to Game"))
    {
        application.client = std::make_unique<GameClient>(cached_username, cached_ip, GAME_PORT);
        application.main_menu_stack.Push(std::make_unique<LoadingScreen>());
    }

    ImGui::Separator();

    if (ImGui::Button("Host Game"))
    {
        application.server = std::make_unique<GameServer>(GAME_PORT, selected_player_count);
        application.client = std::make_unique<GameClient>(cached_username, "localhost", GAME_PORT);
        application.main_menu_stack.Push(std::make_unique<LoadingScreen>());
    }

    ImGui::SliderInt("Number of players", &selected_player_count, 1, MAX_PLAYERS);
    ImGui::Separator();

    auto found = discoverer.GetFoundLobbies();
    if (found.empty())
    {
        ImGui::Text("No local lobbies found...");
    }
    else
    {
        ImGui::Text("Found lobbies:");
    }

    for (auto& lobby : found)
    {
        ImGui::Text("%s (%s): %u/%u", lobby.hostname.c_str(), lobby.ip.c_str(), lobby.current_players, lobby.max_players);
        ImGui::SameLine();

        if (ImGui::Button("Join"))
        {
            application.client = std::make_unique<GameClient>(cached_username, lobby.ip, GAME_PORT);
            application.main_menu_stack.Push(std::make_unique<LoadingScreen>());
        }
    }

    ImGui::End();
}

void LoadingScreen::UpdateMenu(Application& application)
{
    auto connection_state = application.client->GetConnectionResult();

    switch (connection_state)
    {
    case ConnectionResult::WAITING:
        ImGui::Begin("Loading");
        ImGui::Text("Waiting for Connection...");
        ImGui::End();
        break;
    case ConnectionResult::FAIL:
        application.client.reset();
        application.server.reset();
        application.main_menu_stack.Pop();
        break;
    case ConnectionResult::SUCCESS:
        application.main_menu_stack.Pop();
        application.main_menu_stack.Push(std::make_unique<LobbyMenu>());
        break;
    }
}

void LobbyMenu::UpdateMenu(Application& application)
{
    if (application.server && broadcaster == nullptr)
    {
        broadcaster = std::make_unique<LobbyBroadcaster>();
    }

    if (!application.client->GetConnection().IsOpen())
    {
        application.client.reset();
        application.server.reset();
        application.main_menu_stack.Pop();
        return;
    }
    else
    {
        application.client->ProcessMessages(application);

        if (application.in_game)
            return; // Game was started
    }

    ImGui::Begin("Lobby");

    auto lobby = application.client->GetLobbyInfo();
    auto current_player = application.client->GetPlayerIndex();

    uint32_t player_count = lobby.players.size();
    uint32_t player_max = lobby.max_players;

    if (broadcaster)
    {
        broadcaster->UpdateLobbyInfo(player_count, player_max);
    }

    ImGui::Text("Waiting for Players... (%u/%u)", player_count, player_max);
    ImGui::Separator();

    for (uint32_t i = 0; i < lobby.players.size(); i++)
    {
        auto& user_data = lobby.players.at(i);
        ImGui::Text("%u: %s", user_data.player_number + 1, user_data.username.c_str());

        if (current_player == i)
        {
            ImGui::SameLine();
            ImGui::Text("(You)");
        }
    }

    if (application.server)
    {
        ImGui::Text("You are currently the host");

        if (ImGui::Button("Close Server"))
        {
            application.client.reset();
            application.server.reset();
            application.main_menu_stack.Pop();
        }
    }
    else
    {
        if (ImGui::Button("Exit Server"))
        {
            application.client.reset();
            application.server.reset();
            application.main_menu_stack.Pop();
        }
    }

    ImGui::End();
}

void GameHUD::UpdateMenu(Application& application)
{
    auto winner = application.client->GetGameWinner();
    auto controlled = application.client->GetPlayerIndex();

    if (winner)
    {
        ImGui::Begin("Game Over");
        if (winner.value() == controlled)
        {

            ImGui::Text("You Won!");
        }
        else
        {
            Log("You Lost!");
        }
        ImGui::End();
    }

    constexpr std::array<glm::vec2, MAX_PLAYERS> HEALTH_DRAW_POS {
        glm::vec2(50.0f, 50.0f),
        glm::vec2(1550.0f, 850.0f),
        glm::vec2(50.0f, 850.0f),
        glm::vec2(1550.0f, 50.0f),
    };

    constexpr std::array<glm::vec2, MAX_PLAYERS> HEALTH_DRAW_INC {
        glm::vec2(50.0f, 0.0f),
        glm::vec2(-50.0f, 0.0f),
        glm::vec2(50.0f, 0.0f),
        glm::vec2(-50.0f, 0.0f),
    };

    constexpr glm::vec2 HEALTH_SCALE = { 0.5f, 0.5f };

    auto world = application.client->GetWorldInfo();

    for (size_t i = 0; i < world.lives.size(); i++)
    {
        auto& texture = *application.player_assets.at(i).health;
        auto health = world.lives.at(i);

        for (size_t j = 0; j < health; j++)
        {
            Transform2D transform {};
            transform.rotation = 0.0f;
            transform.scale = HEALTH_SCALE;
            transform.translation = HEALTH_DRAW_POS.at(i) + (HEALTH_DRAW_INC.at(i) * (float)j);

            application.renderer.RenderSprite(texture, transform);
        }
    }
}