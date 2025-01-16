#pragma once
#include <glm/vec2.hpp>
#include <unordered_map>
#include <resources/texture.hpp>
#include <optional>

#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/utility.hpp>

struct PlayerData
{
    glm::vec2 position { 0.0f, 0.0f };
    glm::vec2 velocity { 0.0f, 1.0f };
    uint32_t health { 3 };
    float rotation { 0.0f };
};

struct PlayerGlobalTable
{
    uint32_t current_player {};
    uint32_t target_players {};
    std::unordered_map<uint32_t, PlayerData> players {};
};

namespace glm
{
template <typename A>
void serialize(A& ar, vec2& self)
{
    ar(self.x);
    ar(self.y);
}
}

template <typename A>
void serialize(A& ar, PlayerData& self)
{
    ar(self.position);
    ar(self.velocity);
    ar(self.health);
    ar(self.rotation);
}

template <typename A>
void serialize(A& ar, PlayerGlobalTable& self)
{
    ar(self.current_player);
    ar(self.target_players);
    ar(self.players);
}

struct PlayerSkin
{
    Texture player_base_sprite {};
    Texture player_weapon_sprite {};
};

inline std::optional<PlayerSkin> LoadPlayerSkin(SDL_Renderer* renderer, const std::string& base_sprite, const std::string& weapon_sprite)
{
    auto base = Image::FromFile(base_sprite);
    OptCheck(base);

    auto weapon = Image::FromFile(weapon_sprite);
    OptCheck(weapon);

    auto player_base_sprite = Texture::FromImage(renderer, base.value());
    OptCheck(player_base_sprite);

    auto player_weapon_sprite = Texture::FromImage(renderer, weapon.value());
    OptCheck(player_weapon_sprite);

    return PlayerSkin {
        std::move(player_base_sprite.value()),
        std::move(player_weapon_sprite.value())
    };
}