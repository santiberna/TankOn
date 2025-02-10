#include <game/data/game_client.hpp>
#include <game/application.hpp>
#include <game/menus/menus.hpp>

GameClient::GameClient(const std::string& username, const std::string& address, uint16_t port)
{
    player_name = username;
    resolver = std::make_unique<TCPResolver>(
        context_thread.Context(),
        [this](asio::ip::tcp::socket&& s, asio::ip::tcp::endpoint)
        {
            connection = std::make_unique<TCPConnection>(std::move(s));
            connection->Post(AsMessage(CLIENT_SEND_USERNAME, player_name));
            connection->QueueStart();

            result.store(ConnectionResult::SUCCESS);
        },
        [this](std::error_code e)
        {
            Log("Error connecting: {}", e.message());
            result.store(ConnectionResult::FAIL);
        });

    resolver->AttemptConnection(address, port);
    context_thread.Start();
}

void GameClient::ProcessMessages(Application& application)
{
    while (connection && !connection->GetMessages().empty())
    {
        auto msg = connection->GetMessages().pop_front();

        switch (msg.header.type)
        {
        case SERVER_ASSIGN_ID:
        {
            std::scoped_lock<std::mutex> lock { client_mutex };
            player_id = FromMessage<uint32_t>(msg);
            break;
        }
        case SERVER_SEND_LOBBY_DATA:
        {
            std::scoped_lock<std::mutex> lock { client_mutex };
            lobby_state = FromMessage<LobbyInfo>(msg);
            break;
        }
        case SERVER_START_GAME:
        {
            std::scoped_lock<std::mutex> lock { client_mutex };
            world_state = FromMessage<WorldInfo>(msg);

            application.main_menu_stack.Clear();
            application.main_menu_stack.Push(std::make_unique<GameHUD>());
            application.in_game = true;
            break;
        }
        case SERVER_REGISTER_BULLET:
        {
            std::scoped_lock<std::mutex> lock { client_mutex };
            auto data = FromMessage<BulletInfo>(msg);
            world_state.bullets.emplace_back(data);
            break;
        }
        case SERVER_BULLET_HIT:
        {
            std::scoped_lock<std::mutex> lock { client_mutex };
            auto data = FromMessage<HitInfo>(msg);

            world_state.lives.at(data.player_id)--;
            assert(world_state.lives.at(data.player_id) >= 0);

            std::erase_if(world_state.bullets,
                [data](const BulletInfo& b)
                { return b.bullet_id == data.bullet_id; });

            break;
        }
        case BOTH_UPDATE_PLAYER:
        {
            std::scoped_lock<std::mutex> lock { client_mutex };
            auto data = FromMessage<PlayerUpdate>(msg);
            world_state.players.at(data.index) = data.info;
            break;
        }
        case SERVER_DECLARE_WINNER:
        {
            std::scoped_lock<std::mutex> lock { client_mutex };
            game_winner = FromMessage<uint32_t>(msg);
            break;
        }
        default:
            throw;
        }
    }
}