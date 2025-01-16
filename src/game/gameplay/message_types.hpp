#pragma once
#include <cstdint>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <network/containers/message.hpp>

enum MessageType : uint32_t
{
    eNONE,
    eADD_PLAYER,
    eREMOVE_PLAYER,
    eINIT_PLAYER,
    eSTART_GAME,
    eUPDATE_PLAYER,
};

template <typename T>
Message AsMessage(MessageType type, const T& data)
{
    std::stringstream ss {};

    {
        cereal::BinaryOutputArchive ar { ss };
        ar(data);
    }

    return Message(type, ss.str());
}

template <typename T>
T FromMessage(const Message& data)
{
    std::stringstream ss { data.body.data };
    T out {};

    {
        cereal::BinaryInputArchive ar { ss };
        ar(out);
    }

    return out;
}