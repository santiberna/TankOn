#include <game/application.hpp>
#include <utility/colours.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

Application::Application()
{
    renderer = Renderer::Create((uint32_t)WINDOW_SIZE.x, (uint32_t)WINDOW_SIZE.y).value();
    auto* r = renderer.GetRenderer();
    SDL_SetRenderVSync(r, 1);

    main_menu_stack.Push(std::make_unique<MainMenu>());

    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        auto base = Texture::SharedFromImage(
            renderer.GetRenderer(), Image::FromFile(PLAYER_BASE_TEXTURES.at(i)).value());

        auto weapon = Texture::SharedFromImage(
            renderer.GetRenderer(), Image::FromFile(PLAYER_WEAPON_TEXTURES.at(i)).value());

        auto shot = Texture::SharedFromImage(
            renderer.GetRenderer(), Image::FromFile(BULLET_TEXTURES.at(i)).value());

        auto health = Texture::SharedFromImage(
            renderer.GetRenderer(), Image::FromFile(HEART_TEXTURES.at(i)).value());

        player_assets.emplace_back(base, weapon, shot, health);
    }
}

void Application::DoFrame()
{
    DeltaMS deltatime = delta_timer.GetElapsed();
    delta_timer.Reset();

    input.UpdateInput();

    SDL_Event event {};
    while (SDL_PollEvent(&event))
    {
        imgui_shortcuts::ProcessSDLEvent(&event);
        input.ProcessEvent(event);
    }

    renderer.ClearScreen(colour::BLACK);

    if (in_game)
        UpdateGame(deltatime);

    if (!main_menu_stack.Empty())
        main_menu_stack.UpdateTop(*this);
}

void Application::UpdateGame(DeltaMS deltatime)
{
    if (!client->GetConnection().IsOpen())
    {
        client.reset();
        server.reset();

        main_menu_stack.Push(std::make_unique<MainMenu>());
        in_game = false;
        return;
    }
    else
    {
        client->ProcessMessages(*this);
    }

    renderer.ClearScreen(colour::SAND);
    client->ClearDeadBullets();

    auto world_state = client->GetWorldInfo();
    auto controlled = client->GetPlayerIndex();

    auto& current_player = world_state.players.at(controlled);

    {
        glm::vec2 movement {};

        if (input.GetKey(SDLK_W))
        {
            movement += world::UP;
        }
        if (input.GetKey(SDLK_S))
        {
            movement -= world::UP;
        }
        if (input.GetKey(SDLK_D))
        {
            movement += world::RIGHT;
        }
        if (input.GetKey(SDLK_A))
        {
            movement -= world::RIGHT;
        }

        if (glm::epsilonNotEqual(glm::length(movement), 0.0f, glm::epsilon<float>()))
        {
            float rotation_add = movement.x * TANK_STEER * deltatime.count();
            current_player.base_rotation = AngleWrap(current_player.base_rotation + rotation_add);

            auto dir = AngleToVector(current_player.base_rotation + glm::pi<float>() * 0.5f);
            current_player.position += dir * movement.y * TANK_SPEED * deltatime.count();
            current_player.position = glm::clamp(current_player.position, MAP_BOUNDS_MIN, MAP_BOUNDS_MAX);
        }
    }

    {
        auto mouse_pos = input.GetMousePos();
        auto towards_mouse = mouse_pos - current_player.position;
        auto angle = -VectorAngle(world::UP, glm::normalize(towards_mouse));

        current_player.weapon_rotation = angle;
    }

    client->UpdateControlledPlayer(current_player);

    if (shot_cooldown > 0.0f)
    {
        shot_cooldown -= deltatime.count();
    }

    if (input.GetButtonState(SDL_BUTTON_LEFT) == InputState::PRESSED && shot_cooldown <= 0.0f)
    {
        auto now = GetEpochMS();
        auto direction = -AngleToVector(current_player.weapon_rotation + glm::pi<float>() * 0.5f);

        BulletInfo info {};

        info.direction = direction;
        info.start_position = current_player.position + direction * BULLET_SPAWN_OFFSET;
        info.start_time = now.count();
        info.player = controlled;

        client->ShootBullet(info);
        shot_cooldown = BULLET_COOLDOWN_MS;
    }

    for (uint32_t p = 0; p < world_state.players.size(); p++)
    {
        auto& player_data = world_state.players.at(p);
        auto& player_textures = player_assets.at(p);

        Transform2D player = { player_data.position, PLAYER_SIZE, player_data.base_rotation };
        renderer.RenderSprite(*player_textures.base, player);

        player.rotation = player_data.weapon_rotation;
        renderer.RenderSprite(*player_textures.weapon, player);
    }

    for (auto& bullet : world_state.bullets)
    {
        auto now = GetEpochMS();

        Transform2D transform {};
        transform.translation = bullet.start_position + bullet.direction * (float)(now.count() - bullet.start_time) * BULLET_SPEED;
        transform.rotation = -VectorAngle(world::UP, bullet.direction);
        transform.scale = { 1.0f, 1.0f };

        renderer.RenderSprite(*player_assets.at(bullet.player).bullet, transform);
    }
}