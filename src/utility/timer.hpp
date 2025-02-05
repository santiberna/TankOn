#pragma once
#include <chrono>

using DeltaMS = std::chrono::duration<float, std::milli>;
using UintMS = std::chrono::duration<uint32_t, std::milli>;

inline UintMS GetEpochMS()
{
    return std::chrono::duration_cast<UintMS>(std::chrono::system_clock::now().time_since_epoch());
}

class Timer
{
public:
    DeltaMS GetElapsed() const { return std::chrono::high_resolution_clock::now() - start; };
    void Reset() { start = std::chrono::high_resolution_clock::now(); };

private:
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
};