#pragma once
#include <utility/log.hpp>
#include <SDL3/SDL_error.h>

namespace sdl
{

template <typename T, auto Dx>
struct deleter
{
    void operator()(T* v) const
    {
        Dx(v);
    }
};

template <typename T, auto Dx>
using UniquePtr = std::unique_ptr<T, deleter<T, Dx>>;

}

#define SDLCheck(result)     \
    if (!result)             \
    {                        \
        Log(SDL_GetError()); \
        return std::nullopt; \
    }

#define OptCheck(result)     \
    if (!result)             \
    {                        \
        return std::nullopt; \
    }

inline void SDLAbortIfFailed(bool result)
{
    if (!result)
    {
        Log("[SDL ERROR]");
        Log(SDL_GetError());
        std::abort();
    }
}
