#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "StandartIncludes.h"

struct GamepadInfo
{
    SDL_JoystickID ID = -1;
    SDL_Gamepad* Gamepad = nullptr;
    string Name;

    // Новые методы
    SDL_GamepadButton ButtonDown(SDL_Event _event) const;
    SDL_GamepadButton ButtonUp(SDL_Event _event) const;
    bool ButtonHeld(SDL_GamepadButton _button) const;
    string ToString() const;
};

class Gamepad
{
public:
    // Constructors/Destructors
    Gamepad() {}
    virtual ~Gamepad() {}

    // Accessors
    vector<GamepadInfo> GetGamepads() const { return m_gamepads; }

    // Methods
    void Initialize();
    void Detect();
    bool Added(SDL_Event _event);
    bool Removed(SDL_Event _event);

    string ToString();

private:
    bool Add(SDL_JoystickID _joystickId);
    vector<GamepadInfo> m_gamepads;
};

#endif // GAMEPAD_H