#include <game/game.hpp>
#include <utility/imgui_common.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

Game::Game()
{
    renderer = Renderer::Create(1600, 900).value();

    auto* r = renderer.GetRenderer();
    SDL_SetRenderVSync(r, 1);

    player_sprites.emplace_back(LoadPlayerSkin(r, "assets/images/Tanks/TankBaseRed.png", "assets/images/Tanks/TankWeaponRed.png").value());
    player_sprites.emplace_back(LoadPlayerSkin(r, "assets/images/Tanks/TankBaseBlue.png", "assets/images/Tanks/TankWeaponBlue.png").value());
}

void Game::SetGameState(GameStates state)
{
    switch (state)
    {
    case GameStates::MAIN_MENU:
        current_game_state = &main_menu;
        break;
    case GameStates::GAME:
        current_game_state = &game_play_state;
        break;
    case GameStates::LOBBY:
        current_game_state = &lobby_state;
        break;
    }

    current_game_state->OnStateEnter(*this);
}

void Game::ExecuteFrame()
{
    input.UpdateInput();

    SDL_Event event {};
    while (SDL_PollEvent(&event))
    {
        imgui_shortcuts::ProcessSDLEvent(&event);
        input.ProcessEvent(event);
    }

    if (input.ShouldClose())
    {
        should_quit = true;
        return;
    }

    DeltaMS deltatime = delta_timer.GetElapsed();
    delta_timer.Reset();

    current_game_state->Update(*this, deltatime);
}

void Game::DrawPlayer(const Transform2D& camera, const PlayerData& player, uint32_t skin_id)
{
    auto* r = renderer.GetRenderer();

    Transform2D p {};
    p.translation = player.position;
    Transform2D dst = (camera * p);

    auto& skin = player_sprites[skin_id];

    SDL_FRect baseRect = dst.CalcSpriteDst(skin.player_base_sprite.GetSpriteSize());
    SDL_FRect weaponRect = dst.CalcSpriteDst(skin.player_weapon_sprite.GetSpriteSize());

    SDLAbortIfFailed(SDL_RenderTextureRotated(
        r,
        skin.player_base_sprite.handle.get(),
        nullptr,
        &baseRect,
        -glm::degrees(VectorAngle(UP, glm::normalize(player.velocity))),
        nullptr,
        SDL_FLIP_NONE));

    SDLAbortIfFailed(SDL_RenderTextureRotated(
        r,
        skin.player_weapon_sprite.handle.get(),
        nullptr,
        &weaponRect,
        player.rotation,
        nullptr,
        SDL_FLIP_NONE));
}

void Game::UpdatePlayer(PlayerData& player, const Transform2D& camera, DeltaMS deltatime)
{
    glm::vec2 movement {};

    if (input.GetKey(SDLK_W))
    {
        movement += UP;
    }
    if (input.GetKey(SDLK_S))
    {
        movement -= UP;
    }
    if (input.GetKey(SDLK_D))
    {
        movement += RIGHT;
    }
    if (input.GetKey(SDLK_A))
    {
        movement -= RIGHT;
    }

    if (glm::epsilonNotEqual(glm::length(movement), 0.0f, glm::epsilon<float>()))
    {
        movement = glm::normalize(movement);
        player.velocity = movement * deltatime.count();
        player.position += player.velocity;
    }

    glm::vec2 player_screen = camera * player.position;
    glm::vec2 tank_to_cursor = glm::normalize(input.GetMousePos() - player_screen);

    player.rotation = -glm::degrees(VectorAngle(UP, tank_to_cursor));
}
