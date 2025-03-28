#include <game/data/messages.hpp>
#include <magic_enum.hpp>

#if not defined(NDEBUG)
constexpr auto NET_TAG_TEXT = "TankOn";

void AddDebugTag(std::stringstream& str, MessageType type)
{
    str << NET_TAG_TEXT << " | " << magic_enum::enum_name(type) << "\n";
}

void RemoveDebugTag(std::stringstream& str, MessageType type)
{
    std::string tag {};
    std::getline(str, tag);
    assert(tag.starts_with(NET_TAG_TEXT));
}

#else

void AddDebugTag(std::stringstream& str, MessageType type)
{
}

void RemoveDebugTag(std::stringstream& str, MessageType type)
{
}

#endif