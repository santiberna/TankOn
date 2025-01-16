#pragma once
#include <utility/timer.hpp>

class Game;

class BaseGameState
{
public:
    virtual ~BaseGameState() = default;
    virtual void ExecuteFrame(Game& game, DeltaMS delta) = 0;
};