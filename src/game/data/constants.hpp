#pragma once
#include <math/types.hpp>
#include <chrono>
#include <string>
#include <array>

constexpr size_t MAX_PLAYERS = 4;
constexpr glm::vec2 WINDOW_SIZE = { 1600, 900 };

constexpr glm::vec2 MAP_BOUNDS_MIN = { 100, 100 };
constexpr glm::vec2 MAP_BOUNDS_MAX = { 1500, 800 };

constexpr std::chrono::milliseconds LOBBY_TICK_RATE = std::chrono::milliseconds(128);
constexpr std::chrono::milliseconds GAME_TICK_RATE = std::chrono::milliseconds(16);

constexpr std::array<glm::vec2, MAX_PLAYERS> PLAYER_START_POSITIONS {
    glm::vec2(200.0f, 200.0f),
    glm::vec2(1400.0f, 700.0f),
    glm::vec2(200.0f, 700.0f),
    glm::vec2(1400.0f, 200.0f),
};

constexpr uint32_t PLAYER_HEALTH = 5;
constexpr uint32_t BULLET_LIFETIME_MS = 2000;
constexpr float BULLET_SPAWN_OFFSET = 50.0f;
constexpr float BULLET_COOLDOWN_MS = 500.0f;
constexpr float BULLET_SPEED = 0.5f;
constexpr float BULLET_HIT_RADIUS = 50.0f;

constexpr uint32_t END_SCREEN_MS = 5000;

const std::array<std::string, MAX_PLAYERS> PLAYER_BASE_TEXTURES {
    "assets/images/Tanks/TankBaseRed.png",
    "assets/images/Tanks/TankBaseBlue.png",
    "assets/images/Tanks/TankBaseGreen.png",
    "assets/images/Tanks/TankBasePurple.png",
};

const std::array<std::string, MAX_PLAYERS> PLAYER_WEAPON_TEXTURES {
    "assets/images/Tanks/TankWeaponRed.png",
    "assets/images/Tanks/TankWeaponBlue.png",
    "assets/images/Tanks/TankWeaponGreen.png",
    "assets/images/Tanks/TankWeaponPurple.png",
};

const std::array<std::string, MAX_PLAYERS> BULLET_TEXTURES {
    "assets/images/Tanks/ShotRed.png",
    "assets/images/Tanks/ShotBlue.png",
    "assets/images/Tanks/ShotGreen.png",
    "assets/images/Tanks/ShotPurple.png",
};

const std::array<std::string, MAX_PLAYERS> HEART_TEXTURES {
    "assets/images/UI/HeartRed.png",
    "assets/images/UI/HeartBlue.png",
    "assets/images/UI/HeartGreen.png",
    "assets/images/UI/HeartPurple.png",
};

const std::string GAME_FONT = { "assets/fonts/arial.ttf" };

constexpr glm::vec2 PLAYER_SIZE = { 0.5f, 0.5f };
constexpr float TANK_STEER = { 0.003f };
constexpr float TANK_SPEED = { 0.2f };