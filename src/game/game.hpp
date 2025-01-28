#pragma once
#include <game/states/game_states.hpp>
#include <engine/renderer.hpp>
#include <engine/input.hpp>
#include <resources/texture.hpp>
#include <utility/timer.hpp>
#include <game/gameplay/player.hpp>
#include <math/transform2d.hpp>
#include <math/world.hpp>
#include <resources/font.hpp>

class Game
{
public:
    Game();
    void SetGameState(GameStates state);
    void ExecuteFrame();

    void DrawPlayer(const Transform2D& camera, const PlayerData& player, uint32_t skin_id);
    void UpdatePlayer(PlayerData& out_data, const Transform2D& camera, DeltaMS deltatime);

private:
    Timer delta_timer {};

    // States

    MainMenuState main_menu {};
    LobbyState lobby_state {};
    GameplayState game_play_state {};

    BaseGameState* current_game_state = &main_menu;

public:
    // Networking

    std::unique_ptr<GameClient> client {};
    std::unique_ptr<GameServer> server {};

    // Backend

    Renderer renderer {};
    InputData input {};

    // Resources

    std::vector<PlayerSkin> player_sprites {};
    bool should_quit = false;
};