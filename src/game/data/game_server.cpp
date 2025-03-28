#include <game/data/game_server.hpp>

GameServer::GameServer(uint16_t port, uint32_t players)
{
    auto endpoint = asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port);
    max_players = players;

    acceptor = std::make_unique<TCPAcceptor>(
        context_thread.Context(),
        endpoint,
        [this](asio::ip::tcp::socket&& s)
        {
            if (lobby_connections.size() == max_players)
            {
                s.close();
                return;
            }

            GameConnection user {};
            user.connection = std::make_unique<TCPConnection>(std::move(s));
            user.user_data.player_number = lobby_connections.size();

            lobby_connections.emplace_back(std::move(user));
            lobby_connections.back().connection->QueueStart();
            lobby_connections.back().connection->Post(AsMessage(SERVER_ASSIGN_ID, (uint32_t)(lobby_connections.size() - 1)));

            LobbyInfo lobby_state {};
            lobby_state.max_players = max_players;

            for (auto& user : lobby_connections)
                lobby_state.players.emplace_back(user.user_data);

            Broadcast(AsMessage(SERVER_SEND_LOBBY_DATA, lobby_state), nullptr);
        });

    timer = std::make_unique<TickTimer>(
        context_thread.Context(),
        LOBBY_TICK_RATE,
        [this]()
        {
            HandleDisconnects();
            ProcessMessages();

            if (started)
            {
                CheckBulletCollisions();
                CheckWinner();
            }

            auto now = GetEpochMS();
            server_delta.store(now - last_update);
            last_update = now;
        });

    last_update = GetEpochMS();
    context_thread.Start();
}

void GameServer::HandleDisconnects()
{
    auto removed = std::erase_if(lobby_connections, [](const auto& user)
        { return !user.connection->IsOpen(); });

    if (removed != 0)
    {
        LobbyInfo lobby_state {};
        lobby_state.max_players = max_players;

        for (uint32_t i = 0; i < lobby_connections.size(); i++)
        {
            auto& user = lobby_connections.at(i);
            user.connection->Post(AsMessage(SERVER_ASSIGN_ID, i));

            user.user_data.player_number = i;
            lobby_state.players.emplace_back(user.user_data);
        }

        Broadcast(AsMessage(SERVER_SEND_LOBBY_DATA, lobby_state), nullptr);
    }
}

void GameServer::Broadcast(const Message& msg, const TCPConnection* ignore)
{
    for (auto& user : lobby_connections)
    {
        if (user.connection.get() == ignore)
            continue;

        user.connection->Post(msg);
    }
}

void GameServer::ProcessMessages()
{
    if (!started && lobby_connections.size() == max_players)
    {
        started = true;
        acceptor.reset();

        for (uint32_t i = 0; i < lobby_connections.size(); i++)
        {
            world_info.players.emplace_back(
                PlayerInfo { PLAYER_START_POSITIONS.at(i), 0.0f, 0.0f });

            world_info.lives.emplace_back(
                PLAYER_HEALTH);
        }

        Broadcast(AsMessage(SERVER_START_GAME, world_info), nullptr);
        timer->SetTimerExpire(GAME_TICK_RATE);
    }

    for (auto&& user : lobby_connections)
    {
        while (!user.connection->GetMessages().empty())
        {
            auto msg = user.connection->GetMessages().pop_front();

            switch (msg.header.type)
            {
            case CLIENT_SEND_USERNAME:
            {
                user.user_data.username = FromMessage<std::string>(msg);

                LobbyInfo lobby_state {};
                lobby_state.max_players = max_players;

                for (auto& user : lobby_connections)
                {
                    lobby_state.players.emplace_back(user.user_data);
                }

                Broadcast(AsMessage(SERVER_SEND_LOBBY_DATA, lobby_state), nullptr);
                break;
            }
            case CLIENT_SHOOT_BULLET:
            {
                auto data = FromMessage<BulletInfo>(msg);
                data.start_time = GetEpochMS().count();
                data.bullet_id = bullet_id_gen++;

                world_info.bullets.emplace_back(data);
                Broadcast(AsMessage(SERVER_REGISTER_BULLET, data), nullptr);
                break;
            }
            case BOTH_UPDATE_PLAYER:
            {
                auto data = FromMessage<PlayerUpdate>(msg);
                world_info.players.at(data.index) = data.info;

                Broadcast(msg, user.connection.get());
                break;
            }
            case DEBUG_PING:
            {
                user.connection->Post(msg);
                break;
            }
            default:
                throw;
            }
        }
    }
}

void GameServer::CheckBulletCollisions()
{
    auto now = GetEpochMS();

    std::erase_if(world_info.bullets,
        [now](const BulletInfo& i)
        { return i.start_time + BULLET_LIFETIME_MS < now.count(); });

    constexpr float SQR_HIT_RADIUS = BULLET_HIT_RADIUS * BULLET_HIT_RADIUS;

    std::unordered_set<uint32_t> to_remove {};

    auto CheckCollisions = [&](const BulletInfo& bullet)
    {
        glm::vec2 bullet_position = bullet.start_position + bullet.direction * (float)(now.count() - bullet.start_time) * BULLET_SPEED;

        for (size_t j = 0; j < world_info.players.size(); ++j)
        {
            auto& player = world_info.players.at(j);

            if (world_info.lives.at(j) == 0)
                continue;

            if (bullet.player == j)
                continue;

            auto bullet_to_player = bullet_position - player.position;
            auto distance_sqr = (bullet_to_player.x * bullet_to_player.x) + (bullet_to_player.y * bullet_to_player.y);

            if (distance_sqr < SQR_HIT_RADIUS)
            {
                HitInfo info {};
                info.bullet_id = bullet.bullet_id;
                info.player_id = j;

                world_info.lives.at(j)--;
                Broadcast(AsMessage(SERVER_BULLET_HIT, info), nullptr);
                to_remove.emplace(bullet.bullet_id);
                return;
            }
        }
    };

    for (size_t i = 0; i < world_info.bullets.size(); ++i)
    {
        auto& bullet = world_info.bullets.at(i);
        CheckCollisions(bullet);
    }

    std::erase_if(world_info.bullets,
        [&to_remove](const BulletInfo& b)
        { return to_remove.contains(b.bullet_id); });
}

void GameServer::CheckWinner()
{
    uint32_t winner_id = 0;
    uint32_t remaining = 0;

    for (uint32_t i = 0; i < world_info.lives.size(); i++)
    {
        if (world_info.lives.at(i) > 0)
        {
            remaining++;
            winner_id = i;
        }
    }

    if (remaining == 1)
    {
        Broadcast(AsMessage(SERVER_DECLARE_WINNER, winner_id), nullptr);
        timer->Cancel();

        win_sequence_timer = std::make_unique<TickTimer>(
            context_thread.Context(),
            std::chrono::milliseconds(END_SCREEN_MS),
            [this]()
            {
                context_thread.Context().stop();
                lobby_connections.clear();
            });
    }
}