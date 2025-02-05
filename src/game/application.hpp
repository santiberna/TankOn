#pragma once
#include <ui/menu.hpp>
#include <engine/input.hpp>
#include <engine/renderer.hpp>
#include <utility/timer.hpp>

#include <game/data/game_client.hpp>
#include <game/data/game_server.hpp>

#include <utility/imgui_common.hpp>
#include <game/menus/menus.hpp>

#include <resources/texture.hpp>
#include <game/data/constants.hpp>

struct PlayerResources
{
    std::shared_ptr<Texture> base {};
    std::shared_ptr<Texture> weapon {};
    std::shared_ptr<Texture> bullet {};
    std::shared_ptr<Texture> health {};
};

class Application
{
public:
    MenuStack main_menu_stack {};
    Renderer renderer {};
    InputData input {};
    Timer delta_timer {};

    Application();

    void DoFrame();
    void UpdateGame(DeltaMS deltatime);

    // Networking
    std::unique_ptr<GameClient> client {};
    std::unique_ptr<GameServer> server {};

    // Game
    bool in_game = false;
    std::vector<PlayerResources> player_assets {};
    float shot_cooldown = 0.0f;
};