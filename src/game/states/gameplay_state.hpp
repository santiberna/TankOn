#include <game/states/base_state.hpp>
#include <game/gameplay/game_server.hpp>
#include <game/gameplay/game_client.hpp>
#include <resources/texture.hpp>
#include <math/transform2d.hpp>

class GameplayState : public BaseGameState
{
public:
    GameplayState(std::unique_ptr<GameServer>&& server, std::unique_ptr<GameClient>&& client);
    ~GameplayState() override = default;

    void ExecuteFrame(Game& game, DeltaMS deltatime) override;
    void ExitGame(Game& game);

private:
    bool is_host {};
    std::unique_ptr<GameServer> server {};
    std::unique_ptr<GameClient> client {};
    Transform2D camera {};
};