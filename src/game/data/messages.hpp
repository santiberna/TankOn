#pragma once
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

#include <network/utility/message.hpp>
#include <math/types.hpp>
#include <utility/timer.hpp>

enum MessageType
{
    CLIENT_SEND_USERNAME,
    CLIENT_SHOOT_BULLET,

    SERVER_SEND_LOBBY_DATA,
    SERVER_ASSIGN_ID,
    SERVER_START_GAME,
    SERVER_BULLET_HIT,
    SERVER_REGISTER_BULLET,
    SERVER_DECLARE_WINNER,

    BOTH_UPDATE_PLAYER,

    DEBUG_PING
};

struct DebugPingMessage
{
    static constexpr auto MESSAGE = "Hello world, this messages is 64 bytes!!! Yay let celebrate!!!";
    std::string msg = MESSAGE;
    uint32_t timestamp {};

    DebugPingMessage() { timestamp = GetEpochMS().count(); }

    template <typename A>
    void serialize(A& ar)
    {
        ar(msg);
        ar(timestamp);
    }
};

struct LobbyDiscoverInfo
{
    std::string hostname {};
    std::string ip {};

    uint32_t current_players {};
    uint32_t max_players {};

    template <typename A>
    void serialize(A& ar)
    {
        ar(hostname);
        ar(ip);
        ar(current_players);
        ar(max_players);
    }
};

struct UserData
{
    std::string username = "Loading...";
    uint32_t player_number;

    template <typename A>
    void serialize(A& ar)
    {
        ar(username);
        ar(player_number);
    }
};

struct LobbyInfo
{
    uint32_t max_players {};
    std::vector<UserData> players;

    template <typename A>
    void serialize(A& ar)
    {
        ar(max_players);
        ar(players);
    }
};

struct PlayerInfo
{
    glm::vec2 position {};
    float base_rotation {};
    float weapon_rotation {};

    template <typename A>
    void serialize(A& ar)
    {
        ar(position);
        ar(base_rotation);
        ar(weapon_rotation);
    }
};

struct BulletInfo
{
    glm::vec2 start_position {};
    glm::vec2 direction {};
    uint32_t start_time {};
    uint32_t player {};
    uint32_t bullet_id {};

    template <typename A>
    void serialize(A& ar)
    {
        ar(start_position);
        ar(direction);
        ar(start_time);
        ar(player);
        ar(bullet_id);
    }
};

struct HitInfo
{
    uint32_t player_id {};
    uint32_t bullet_id {};

    template <typename A>
    void serialize(A& ar)
    {
        ar(player_id);
        ar(bullet_id);
    }
};

struct WorldInfo
{
    std::vector<PlayerInfo> players {};
    std::vector<BulletInfo> bullets {};
    std::vector<uint32_t> lives {};

    template <typename A>
    void serialize(A& ar)
    {
        ar(players);
        ar(bullets);
        ar(lives);
    }
};

struct PlayerUpdate
{
    uint32_t index {};
    PlayerInfo info {};

    template <typename A>
    void serialize(A& ar)
    {
        ar(index);
        ar(info);
    }
};

template <typename T>
std::string SerializeStruct(const T& data)
{
    std::stringstream ss {};

    {
        cereal::BinaryOutputArchive ar { ss };
        ar(data);
    }

    return ss.str();
}

template <typename T>
T DeserializeStruct(const std::string& data)
{
    std::stringstream ss { data };
    T out {};

    {
        cereal::BinaryInputArchive ar { ss };
        ar(out);
    }

    return out;
}

template <typename T>
Message AsMessage(MessageType type, const T& data)
{
    return Message(static_cast<uint32_t>(type), SerializeStruct(data));
}

template <typename T>
T FromMessage(const Message& data)
{
    return DeserializeStruct<T>(data.body.data);
}