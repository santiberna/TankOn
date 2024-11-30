#pragma once
#include <memory>
#include <utility/log.hpp>
#include <SDL3/SDL_error.h>

namespace hpp
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
using unique_ptr = std::unique_ptr<T, deleter<T, Dx>>;

}

#define CheckSDL(result)     \
    if (!result)             \
    {                        \
        Log(SDL_GetError()); \
        return std::nullopt; \
    }

inline void CheckErrorSDL(bool result)
{
    if (!result)
    {
        Log(SDL_GetError());
        std::abort();
    }
}