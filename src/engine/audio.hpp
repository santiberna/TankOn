#include <SDL3_mixer/SDL_mixer.h>
#include <utility/log.hpp>
#include <utility/sdl_helpers.hpp>
#include <optional>

class Music
{
public:
    static std::optional<Music> Create(const std::string& path)
    {
        auto* music = Mix_LoadMUS(path.c_str());
        SDLCheck(music);

        Music out {};
        out.handle = decltype(out.handle) { music };
        return out;
    }

    sdl::UniquePtr<Mix_Music, Mix_FreeMusic> handle {};
};