#pragma once
#include <game/context.hpp>
#include <game/input.hpp>
#include <resources/texture.hpp>
#include <utility/timer.hpp>
#include <game/player.hpp>
#include <math/transform2d.hpp>
#include <math/world.hpp>

enum class GameState
{
    eMENU,
    eLOBBY,
    eGAME
};

class Game
{
public:
    GameState state = GameState::eMENU;

    Context context {};
    InputData input_handler {};

    Texture player_base_sprite {};
    Texture player_weapon_sprite {};

    Transform2D camera {};
    PlayerData player {};

    Timer delta_timer {};
    bool should_quit = false;

    Game();

    void ClearScreen(const glm::vec3& colour);
    void DrawMenuUI();

    void ProcessAllEvents();
    void RenderObjects();
    void UpdatePlayer();
};