#include <game/game.hpp>
#include <utility/imgui_common.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

Game::Game()
{
    context = Context::Create().value();
    auto* renderer = context.GetRenderer();

    Image base = Image::FromFile("assets/images/Tanks/TankBaseRed.png").value();
    Image weapon = Image::FromFile("assets/images/Tanks/TankWeaponRed.png").value();

    player_base_sprite = Texture::FromImage(renderer, base).value();
    player_weapon_sprite = Texture::FromImage(renderer, weapon).value();

    camera.translation = { kScreenWidth * 0.5f, kScreenHeight * 0.5f };
    SDL_SetRenderVSync(renderer, 1);

    player.velocity = UP;
}

void Game::ClearScreen(const glm::vec3& colour)
{
    uint8_t red = static_cast<uint8_t>(colour.x * 255.0f);
    uint8_t green = static_cast<uint8_t>(colour.y * 255.0f);
    uint8_t blue = static_cast<uint8_t>(colour.z * 255.0f);

    SDL_SetRenderDrawColor(context.GetRenderer(), red, green, blue, 0xFF);
    SDLAbortIfFailed(SDL_RenderClear(context.GetRenderer()));
}

void Game::DrawMenuUI()
{
    ImGui::Begin("Main Menu");

    ImGui::Text("IP to connect:");
    ImGui::SameLine();

    std::string ipv4 {};
    ImGui::InputText("Input IP", &ipv4);

    ImGui::Button("Connect to Game");

    ImGui::Separator();
    ImGui::Button("Host Game");

    ImGui::End();
}

void Game::ProcessAllEvents()
{
    SDL_Event event {};
    while (SDL_PollEvent(&event))
    {
        imgui_shortcuts::ProcessSDLEvent(&event);
        input_handler.ProcessEvent(event);
    }

    if (input_handler.ShouldClose())
    {
        should_quit = true;
    }
}

void Game::RenderObjects()
{
    auto* renderer = context.GetRenderer();

    Transform2D p {};
    p.translation = player.position;
    Transform2D dst = (camera * p);

    SDL_FRect baseRect = dst.CalcSpriteDst(player_base_sprite.GetSpriteSize());
    SDL_FRect weaponRect = dst.CalcSpriteDst(player_weapon_sprite.GetSpriteSize());

    glm::vec2 tank_to_cursor = glm::normalize(input_handler.GetMousePos() - dst.translation);
    float weapon_rotation = -glm::degrees(VectorAngle(UP, tank_to_cursor));

    float base_rotation = -glm::degrees(VectorAngle(UP, glm::normalize(player.velocity)));

    SDL_RenderTextureRotated(renderer, player_base_sprite.handle.get(), nullptr, &baseRect, base_rotation, nullptr, SDL_FLIP_NONE);
    SDLAbortIfFailed(SDL_RenderTextureRotated(renderer, player_weapon_sprite.handle.get(), nullptr, &weaponRect, weapon_rotation, nullptr, SDL_FLIP_NONE));
}

void Game::UpdatePlayer()

{
    auto dt = delta_timer.GetElapsed();
    delta_timer.Reset();

    glm::vec2 movement {};

    if (input_handler.GetKey(SDLK_W))
    {
        movement += UP;
    }
    if (input_handler.GetKey(SDLK_S))
    {
        movement -= UP;
    }
    if (input_handler.GetKey(SDLK_D))
    {
        movement += RIGHT;
    }
    if (input_handler.GetKey(SDLK_A))
    {
        movement -= RIGHT;
    }

    if (glm::epsilonNotEqual(glm::length(movement), 0.0f, glm::epsilon<float>()))
    {
        movement = glm::normalize(movement);
        player.velocity = movement * dt.count();
        player.position += player.velocity;
    }
}
