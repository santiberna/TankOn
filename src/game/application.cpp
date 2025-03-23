#include <game/application.hpp>
#include <utility/colours.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

#include <ui/widgets/debug_rect.hpp>
#include <ui/widgets/sprite.hpp>
#include <ui/widgets/text_box.hpp>
#include <ui/canvas.hpp>

Application::Application()
{
    renderer = Renderer::Create((uint32_t)WINDOW_SIZE.x, (uint32_t)WINDOW_SIZE.y).value();
    Log("[INFO] Initialized Window Successfully!");
    auto* r = renderer.GetRenderer();

    SDLAbortIfFailed(SDL_SetRenderVSync(r, 1));
    SDLAbortIfFailed(SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND));
    Log("[INFO] Initialized Renderer Successfully!");

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
    Log("[INFO] Loaded all Player Assets Successfully!");

    FontLoadInfo info {};
    info.codepoint_ranges.emplace_back(unicode::ASCII_CODESET);
    info.codepoint_ranges.emplace_back(unicode::LATIN_SUPPLEMENT_CODESET);

    game_font = Font::SharedFromFile(renderer.GetRenderer(), GAME_FONT, FontLoadInfo {});
    Log("[INFO] Loaded Game font Successfully!");

    // ui_canvas = SetupCanvas();

    input.OnCloseRequested().connect([this]()
        { close_game = true; });
}

Canvas Application::SetupCanvas()
{
    Canvas canvas {};

    auto element = std::make_unique<UIDebugRect>();
    element->active = true;
    element->colour = colour::WHITE;
    element->transform.size = { 0.5f, 0.5f };

    auto child_element = std::make_unique<TextBox>(game_font, unicode::FromASCII("Hello World!"));
    child_element->transform.size = { 1.0f, 1.0f };

    input.OnTextInput().connect([text = child_element.get()](auto& c)
        { 
        if (c.front() == unicode::BACKSPACE_CODEPOINT && text->text.size()) {
            text->text.pop_back();
        } else if (c.front() != unicode::BACKSPACE_CODEPOINT){
        text->text.append(c.c_str());
        } });

    element->children.emplace_back(std::move(child_element));
    canvas.elements.emplace_back(std::move(element));

    return canvas;
}

void Application::HandleInput()
{
    input.UpdateInput();

    SDL_Event event {};
    while (SDL_PollEvent(&event))
    {
        imgui_shortcuts::ProcessSDLEvent(&event);
        input.ProcessEvent(event);
    }
}

void Application::DoFrame()
{
    DeltaMS deltatime = delta_timer.GetElapsed();
    delta_timer.Reset();

    renderer.ClearScreen(colour::BLACK);

    if (in_game)
        UpdateGame(deltatime);

    if (!main_menu_stack.Empty())
        main_menu_stack.UpdateTop(*this);

    // ui_canvas.RenderCanvas(renderer);
}

void Application::UpdateGame(DeltaMS deltatime)
{
    if (!client->GetConnection().IsOpen())
    {
        client.reset();
        server.reset();
        in_game = false;

        main_menu_stack.Pop();
        main_menu_stack.Push(std::make_unique<MainMenu>());

        return;
    }
    else
    {
        client->ProcessMessages(*this);
    }

    renderer.ClearScreen(colour::SAND);
    client->ClearDeadBullets();

    {
        ImGui::Begin("Debug Menu");
        ImGui::Text("Frametime: %f", deltatime.count());
        ImGui::Text("Ping: %u", client->GetPingMS());
        ImGui::Text("Incoming Messages: %zi", client->GetConnection().GetMessages().count());

        if (server)
        {
            ImGui::Separator();
            ImGui::Text("Server Deltatime: %u", server->GetServerDeltatime().count());
        }

        ImGui::End();
    }

    auto world_state = client->GetWorldInfo();
    auto controlled = client->GetPlayerIndex();

    auto& current_player = world_state.players.at(controlled);

    if (world_state.lives.at(controlled) > 0)
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

        auto mouse_pos = input.GetMousePos();
        auto towards_mouse = mouse_pos - current_player.position;
        auto angle = -VectorAngle(world::UP, glm::normalize(towards_mouse));

        current_player.weapon_rotation = angle;
        client->UpdateControlledPlayer(current_player);

        if (shot_cooldown > 0.0f)
        {
            shot_cooldown -= deltatime.count();
        }

        if (input.GetButtonState(SDL_BUTTON_LEFT) == InputState::PRESSED && shot_cooldown <= 0.0f)
        {
            BulletInfo info {};

            info.direction = glm::normalize(towards_mouse);
            info.start_position = current_player.position + info.direction * BULLET_SPAWN_OFFSET;
            info.player = controlled;

            client->ShootBullet(info);
            shot_cooldown = BULLET_COOLDOWN_MS;
        }
    }

    for (uint32_t p = 0; p < world_state.players.size(); p++)
    {
        if (world_state.lives.at(p) == 0)
            continue;

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
        Log("{} {}", transform.translation.x, transform.translation.y)
    }
}