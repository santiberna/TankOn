#include <SDL3/SDL_main.h>
#include <context/context.hpp>
#include <context/input.hpp>
#include <resources/texture.hpp>
#include <utility/timer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>

#include <math/world.hpp>
#include <math/transform2d.hpp>

int main(int argc, char* args[])
{
    bool success = SDL_Init(SDL_INIT_VIDEO);
    CheckErrorSDL(success);

    Context c = Context::Create().value();
    InputData input {};

    Image base = Image::FromFile("assets/images/Tanks/TankBaseRed.png").value();
    Image weapon = Image::FromFile("assets/images/Tanks/TankWeaponRed.png").value();

    Texture base_sprite = Texture::FromImage(c.renderer.get(), base).value();
    Texture weapon_sprite = Texture::FromImage(c.renderer.get(), weapon).value();

    Transform2D camera {};
    camera.translation = { kScreenWidth * 0.5f, kScreenHeight * 0.5f };

    Transform2D transform {};

    SDL_SetRenderVSync(c.renderer.get(), 1);

    Timer delta {};

    while (true)
    {
        auto dt = delta.GetElapsed();
        delta.Reset();

        SDL_Event event {};
        while (SDL_PollEvent(&event))
        {
            input.ProcessEvent(event);
        }

        if (input.ShouldClose())
        {
            break;
        }

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
        if (input.GetKey(SDLK_E))
        {
            transform.scale += glm::vec2 { 1.0f, 1.0f } * dt.count() * 0.001f;
        }
        if (input.GetKey(SDLK_Q))
        {
            transform.scale -= glm::vec2 { 1.0f, 1.0f } * dt.count() * 0.001f;
        }

        if (glm::epsilonNotEqual(glm::length(movement), 0.0f, glm::epsilon<float>()))
        {
            movement = glm::normalize(movement);
        }

        transform.translation += movement * dt.count();

        SDL_SetRenderDrawColor(c.renderer.get(), 0xE7, 0xD4, 0xA9, 0xFF);
        CheckErrorSDL(SDL_RenderClear(c.renderer.get()));
        SDL_SetRenderDrawColor(c.renderer.get(), 0x00, 0xFF, 0x00, 0xFF);

        Transform2D dst = (camera * transform);
        SDL_FRect baseRect = dst.CalcSpriteDst(base_sprite.GetSpriteSize());
        SDL_FRect weaponRect = dst.CalcSpriteDst(weapon_sprite.GetSpriteSize());

        glm::vec2 tank_to_cursor = glm::normalize(input.GetMousePos() - dst.translation);
        float weapon_rotation = -glm::degrees(VectorAngle(UP, tank_to_cursor));

        float base_rotation = -glm::degrees(VectorAngle(UP, movement));

        CheckErrorSDL(SDL_RenderTextureRotated(c.renderer.get(), base_sprite.handle.get(), nullptr, &baseRect, base_rotation, nullptr, SDL_FLIP_NONE));
        CheckErrorSDL(SDL_RenderTextureRotated(c.renderer.get(), weapon_sprite.handle.get(), nullptr, &weaponRect, weapon_rotation, nullptr, SDL_FLIP_NONE));
        CheckErrorSDL(SDL_RenderPresent(c.renderer.get()));
    }

    SDL_Quit();
    return 0;
}
