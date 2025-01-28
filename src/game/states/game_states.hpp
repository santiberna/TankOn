#pragma once
#include <utility/timer.hpp>
#include <game/gameplay/game_server.hpp>
#include <game/gameplay/game_client.hpp>
#include <math/transform2d.hpp>

class Game;

enum class GameStates
{
    MAIN_MENU,
    LOBBY,
    GAME
};

class BaseGameState
{
public:
    virtual ~BaseGameState() = default;

    virtual void OnStateEnter([[maybe_unused]] Game& game) { };
    virtual void Update(Game& game, DeltaMS delta) = 0;
};

// Main Menu

class MainMenuState : public BaseGameState
{
public:
    MainMenuState() = default;
    ~MainMenuState() override = default;

    void Update(Game& game, DeltaMS deltatime) override;

    void AttemptStartServer(Game& game);
    void AttemptJoinServer(Game& game);

private:
    std::string cached_ip {};
    GameInitInfo cached_startup_info {};
};

// Gameplay

class GameplayState : public BaseGameState
{
public:
    GameplayState();
    ~GameplayState() override = default;

    void Update(Game& game, DeltaMS deltatime) override;
    void ExitGame(Game& game);

    void UpdatePlayer(Game& game, PlayerData& out_data, DeltaMS deltatime);

    Transform2D camera;
};

// Lobby

class LobbyState : public BaseGameState
{
public:
    LobbyState() = default;
    ~LobbyState() override = default;

    void Update(Game& game, DeltaMS deltatime) override;
    void StartGamemode(Game& game);
    void ExitLobby(Game& game);
};