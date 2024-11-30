#include <SDL3/SDL_main.h>
#include <context/context.hpp>
#include <resources/texture.hpp>
#include <utility/timer.hpp>
#include <glm/glm.hpp>

int main(int argc, char* args[])
{
    Context c = Context::Create().value();

    Image base = Image::FromFile("assets/images/Tanks/TankBaseRed.png").value();
    Image weapon = Image::FromFile("assets/images/Tanks/TankWeaponRed.png").value();

    Texture t = Texture::FromImage(c.renderer.get(), base).value();

    SDL_SetRenderVSync(c.renderer.get(), 1);

    bool run = true;
    Timer delta {};

    while (run)
    {
        auto dt = delta.GetElapsed();
        delta.Reset();

        SDL_Event event {};
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                run = false;
            }
        }

        SDL_SetRenderDrawColor(c.renderer.get(), 0xE7, 0xD4, 0xA9, 0xFF);
        CheckErrorSDL(SDL_RenderClear(c.renderer.get()));
        SDL_SetRenderDrawColor(c.renderer.get(), 0x00, 0xFF, 0x00, 0xFF);

        static float angle = 0.0f;
        angle += dt.count() * 0.1f;

        SDL_FRect dstRect = t.GetSpriteRect({ 0.0f, 0.0f });

        CheckErrorSDL(SDL_RenderRect(c.renderer.get(), &dstRect));
        CheckErrorSDL(SDL_RenderTextureRotated(c.renderer.get(), t.handle.get(), nullptr, &dstRect, angle, nullptr, SDL_FLIP_NONE));

        CheckErrorSDL(SDL_RenderPresent(c.renderer.get()));
    }

    return 0;
}
