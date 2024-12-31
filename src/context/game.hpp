#pragma once
#include <context/context.hpp>
#include <context/input.hpp>
#include <resources/texture.hpp>
#include <utility/timer.hpp>
#include <math/transform2d.hpp>
#include <math/world.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>

class Game
{
public:
    Context c {};
    InputData input_handler {};

    Texture player_base_sprite {};
    Texture player_weapon_sprite {};

    Transform2D camera {};
    Transform2D player {};

    Timer delta_timer {};
    bool should_quit = false;

    Game()
    {
        bool success = SDL_Init(SDL_INIT_VIDEO);
        CheckErrorSDL(success);

        c = Context::Create().value();

        Image base = Image::FromFile("assets/images/Tanks/TankBaseRed.png").value();
        Image weapon = Image::FromFile("assets/images/Tanks/TankWeaponRed.png").value();

        player_base_sprite = Texture::FromImage(c.renderer.get(), base).value();
        player_weapon_sprite = Texture::FromImage(c.renderer.get(), weapon).value();

        camera.translation = { kScreenWidth * 0.5f, kScreenHeight * 0.5f };
        SDL_SetRenderVSync(c.renderer.get(), 1);
    }

    ~Game()
    {
        player_base_sprite = {};
        player_weapon_sprite = {};

        c = {};
        SDL_Quit();
    }

    void Update()
    {
        auto dt = delta_timer.GetElapsed();
        delta_timer.Reset();

        SDL_Event event {};
        while (SDL_PollEvent(&event))
        {
            input_handler.ProcessEvent(event);
        }

        if (input_handler.ShouldClose())
        {
            should_quit = true;
            return;
        }

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
        if (input_handler.GetKey(SDLK_E))
        {
            player.scale += glm::vec2 { 1.0f, 1.0f } * dt.count() * 0.001f;
        }
        if (input_handler.GetKey(SDLK_Q))
        {
            player.scale -= glm::vec2 { 1.0f, 1.0f } * dt.count() * 0.001f;
        }

        if (glm::epsilonNotEqual(glm::length(movement), 0.0f, glm::epsilon<float>()))
        {
            movement = glm::normalize(movement);
        }

        player.translation += movement * dt.count();

        SDL_SetRenderDrawColor(c.renderer.get(), 0xE7, 0xD4, 0xA9, 0xFF);
        CheckErrorSDL(SDL_RenderClear(c.renderer.get()));
        SDL_SetRenderDrawColor(c.renderer.get(), 0x00, 0xFF, 0x00, 0xFF);

        Transform2D dst = (camera * player);
        SDL_FRect baseRect = dst.CalcSpriteDst(player_base_sprite.GetSpriteSize());
        SDL_FRect weaponRect = dst.CalcSpriteDst(player_weapon_sprite.GetSpriteSize());

        glm::vec2 tank_to_cursor = glm::normalize(input_handler.GetMousePos() - dst.translation);
        float weapon_rotation = -glm::degrees(VectorAngle(UP, tank_to_cursor));

        float base_rotation = -glm::degrees(VectorAngle(UP, movement));

        CheckErrorSDL(SDL_RenderTextureRotated(c.renderer.get(), player_base_sprite.handle.get(), nullptr, &baseRect, base_rotation, nullptr, SDL_FLIP_NONE));
        CheckErrorSDL(SDL_RenderTextureRotated(c.renderer.get(), player_weapon_sprite.handle.get(), nullptr, &weaponRect, weapon_rotation, nullptr, SDL_FLIP_NONE));
        CheckErrorSDL(SDL_RenderPresent(c.renderer.get()));
    }
};