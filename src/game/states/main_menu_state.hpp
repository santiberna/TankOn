#include <game/states/base_state.hpp>
#include <game/gameplay/game_server.hpp>

class MainMenuState : public BaseGameState
{
public:
    MainMenuState() = default;
    ~MainMenuState() override = default;

    void ExecuteFrame(Game& game, DeltaMS deltatime) override;

    void AttemptStartServer(Game& game);
    void AttemptJoinServer(Game& game);

private:
    std::string cached_ip {};
    GameInitInfo cached_startup_info {};
};