#pragma once
#include <game/menu.hpp>
#include <engine/renderer.hpp>
#include <utility/timer.hpp>
#include <game/data/game_client.hpp>
#include <game/data/game_server.hpp>
#include <utility/imgui_common.hpp>
#include <game/menus/menus.hpp>
#include <resources/texture.hpp>
#include <game/data/constants.hpp>
#include <resources/font.hpp>
#include <ui/canvas.hpp>
#include <input/event_system.hpp>
#include <game/data/lobby_discoverer.hpp>
#include <game/data/lobby_broadcaster.hpp>

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
    Canvas ui_canvas {};
    MenuStack main_menu_stack {};
    Renderer renderer {};
    InputEventSystem input {};
    Timer delta_timer {};

    Application();

    void CreateCanvas();
    void HandleInput();
    void DoFrame();
    void UpdateGame(DeltaMS deltatime);

    // Networking
    std::unique_ptr<GameClient> client {};
    std::unique_ptr<GameServer> server {};

    std::unique_ptr<LobbyDiscoverer> discoverer {};
    std::unique_ptr<LobbyBroadcaster> broadcaster {};

    // Game
    bool close_game = false;
    bool in_game = false;
    std::vector<PlayerResources> player_assets {};
    float shot_cooldown = 0.0f;

    // Font
    std::shared_ptr<Font> game_font;

private:
    Canvas SetupCanvas();
};