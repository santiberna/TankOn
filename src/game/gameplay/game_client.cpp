#include <game/gameplay/game_client.hpp>

void GameClient::ProcessMessage(TCPConnection& sender, const Message& message)
{
    switch (message.header.message_type)
    {
    case MessageType::eINIT_PLAYER:
        info = FromMessage<PlayerGlobalTable>(message);
        break;
    case MessageType::eREMOVE_PLAYER:
        info.players.erase(FromMessage<uint32_t>(message));
        break;
    case MessageType::eADD_PLAYER:
        info.players.emplace(FromMessage<std::pair<uint32_t, PlayerData>>(message));
        break;
    case MessageType::eSTART_GAME:
        has_started = true;
        break;
    case MessageType::eUPDATE_PLAYER:
    {
        auto [id, data] = FromMessage<std::pair<uint32_t, PlayerData>>(message);
        info.players[id] = data;
        break;
    }
    }
}