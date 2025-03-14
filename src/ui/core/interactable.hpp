#pragma once
#include <ui/core/element.hpp>
#include <input/event_system.hpp>

enum class InteractState
{
    NONE,
    HOVERED,
    PRESSED
};

class UIInteractable : public UIElement
{
public:
    UIInteractable(InputEventSystem& input)
    {
        auto press_handler = [this, &input](bool pressed) 
        {
            
        }

        input.OnButtonClick(SDL_BUTTON_LEFT).connect()
    }

    virtual ~UIInteractable() = default;
protected:

};