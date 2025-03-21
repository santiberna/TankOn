#pragma once
#include <game/menu.hpp>
#include <string>
#include <game/data/lobby_discovery.hpp>

class MainMenu : public MenuInterface
{
public:
    virtual ~MainMenu() = default;
    virtual void UpdateMenu(Application& application) override;

private:
    int selected_player_count = 4;
    std::string cached_ip {};
    std::string cached_username = "New Player";
    LobbyDiscoverer discoverer {};
};

class LoadingScreen : public MenuInterface
{
public:
    virtual ~LoadingScreen() = default;
    virtual void UpdateMenu(Application& application) override;
};

class LobbyMenu : public MenuInterface
{
public:
    virtual ~LobbyMenu() = default;
    virtual void UpdateMenu(Application& application) override;

    std::unique_ptr<LobbyBroadcaster> broadcaster {};
};

class GameHUD : public MenuInterface
{
public:
    virtual ~GameHUD() = default;
    virtual void UpdateMenu(Application& application) override;
};