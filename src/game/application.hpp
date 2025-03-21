#pragma once
#include <game/menu.hpp>
#include <engine/renderer.hpp>
#include <utility/time.hpp>
#include <game/data/game_client.hpp>
#include <game/data/game_server.hpp>
#include <utility/imgui_common.hpp>
#include <game/menus/menus.hpp>
#include <resources/texture.hpp>
#include <game/data/constants.hpp>
#include <resources/font.hpp>
#include <ui/core/menu.hpp>
#include <input/event_system.hpp>

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
    Menu ui_canvas {};
    MenuStack main_menu_stack {};
    Renderer renderer {};
    std::unique_ptr<InputEventSystem> input {};
    Timer delta_timer {};

    Application();

    void CreateCanvas();
    void HandleInput();
    void DoFrame();
    void UpdateGame(DeltaMS deltatime);

    // Networking
    std::unique_ptr<GameClient> client {};
    std::unique_ptr<GameServer> server {};

    // Game
    bool close_game = false;
    bool in_game = false;
    std::vector<PlayerResources> player_assets {};
    float shot_cooldown = 0.0f;

    // Font
    std::shared_ptr<Font> game_font;

    // Input slots
    glm::vec2 player_movement {};
    glm::vec2 mouse_pos {};
    CursorState cursor {};

private:
    Menu SetupCanvas();
};