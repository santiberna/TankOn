#include <game/gameplay/game_server.hpp>

GameServer::GameServer(const GameInitInfo& info)
    : TCPServer(SERVER_DEFAULT_PORT)
{
    player_data.target_players = info.numPlayers;
    server_thread = std::thread([this]()
        { Run(); });

    lobby_check_timer = std::make_unique<asio::steady_timer>(context, std::chrono::milliseconds(100));
    lobby_check_timer->async_wait(BindMember(this, &GameServer::CheckLobbyConnections));
}

GameServer::~GameServer()
{
    Stop();
    NotifyServer();

    if (server_thread.joinable())
        server_thread.join();
}

bool GameServer::OnAcceptConnection(TCPConnection& sender)
{
    if (!ingame)
    {
        uint32_t player_id = player_data.players.size() + 1;
        PlayerData data {};

        player_data.current_player = player_id;
        player_data.players[player_id] = data;

        Broadcast(AsMessage(MessageType::eADD_PLAYER, std::make_pair(player_id, data)), &sender);
        Send(sender, AsMessage(MessageType::eINIT_PLAYER, player_data));

        return true;
    }
    return false;
}

void GameServer::CheckLobbyConnections(std::error_code e)
{
    for (auto& c : connections)
    {
        if (!c->GetSocket().is_open())
        {
            RemoveConnection(*c);
            break;
        }
    }

    if (!ingame && player_data.players.size() == player_data.target_players)
    {
        Log("Starting Game...");
        StartGame();
        ingame = true;
    }

    lobby_check_timer->async_wait(BindMember(this, &GameServer::CheckLobbyConnections));
}

void GameServer::StartGame()
{
    Broadcast(Message(MessageType::eSTART_GAME));
}

void GameServer::OnDisconnection(TCPConnection& sender)
{
    Broadcast(AsMessage(MessageType::eREMOVE_PLAYER, sender.GetId()), &sender);
}

void GameServer::ProcessMessage(TCPConnection& sender, const Message& message)
{
    switch (message.header.message_type)
    {
    case MessageType::eUPDATE_PLAYER:
    {
        auto [id, data] = FromMessage<std::pair<uint32_t, PlayerData>>(message);
        player_data.players[id] = data;
        Broadcast(message, &sender);
    }
    break;
    default:
        break;
    }
}