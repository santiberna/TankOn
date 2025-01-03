#include <SDL3/SDL_events.h>
#include <unordered_map>
#include <glm/vec2.hpp>

using SDL_Mousebutton = Uint8;

class InputData
{
public:
    void ProcessEvent(const SDL_Event& event);
    bool GetKey(SDL_Keycode code) { return key_state[code]; }
    bool ShouldClose() const { return exit_code; }
    glm::vec2 GetMousePos() { return mouse_pos; }

private:
    std::unordered_map<SDL_Keycode, bool> key_state {};
    std::unordered_map<SDL_Mousebutton, bool> button_state {};

    glm::vec2 mouse_pos { -1.0f, -1.0f };
    bool exit_code = false;
};