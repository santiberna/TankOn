#pragma once
#include <chrono>

using DeltaMS = std::chrono::duration<float, std::milli>;

class Timer
{
public:
    DeltaMS GetElapsed() const { return std::chrono::high_resolution_clock::now() - start; };
    void Reset() { start = std::chrono::high_resolution_clock::now(); };

private:
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
};