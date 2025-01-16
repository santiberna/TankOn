#pragma once
#include <game/states/base_state.hpp>
#include <game/backend/context.hpp>
#include <game/backend/input.hpp>
#include <resources/texture.hpp>
#include <utility/timer.hpp>
#include <game/gameplay/player.hpp>
#include <math/transform2d.hpp>
#include <math/world.hpp>

class Game
{
public:
    std::unique_ptr<BaseGameState> game_state;

    void SetGameState(BaseGameState* state)
    {
        game_state.reset(state);
    }

    void ExecuteFrame()
    {
        if (game_state == nullptr)
        {
            return;
        }

        DeltaMS deltatime = delta_timer.GetElapsed();
        delta_timer.Reset();

        game_state->ExecuteFrame(*this, deltatime);
    }

    Context context {};
    InputData input_handler {};
    std::vector<PlayerSkin> player_sprites {};

    Timer delta_timer {};
    bool should_quit = false;

    Game();

    void ClearScreen(const glm::vec3& colour);
    void DrawPlayer(const Transform2D& camera, const PlayerData& player, uint32_t skin_id);
    void UpdatePlayer(PlayerData& out_data, const Transform2D& camera, DeltaMS deltatime);
    void ProcessAllEvents();
};