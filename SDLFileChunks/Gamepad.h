#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "StandartIncludes.h"

struct GamepadInfo
{
    SDL_JoystickID ID = -1;
    SDL_Gamepad* Gamepad = nullptr;
    string Name;

    // Переменные для хранения состояния стиков
    SDL_FPoint LeftStick = { 0.0f, 0.0f };
    SDL_FPoint RightStick = { 0.0f, 0.0f };

    // Методы обработки кнопок
    SDL_GamepadButton ButtonDown(SDL_Event _event) const;
    SDL_GamepadButton ButtonUp(SDL_Event _event) const;
    bool ButtonHeld(SDL_GamepadButton _button) const;

    
    bool ProcessMotion(SDL_Event _event);
    SDL_FPoint& GetLeftStick();
    SDL_FPoint& GetRightStick();

    string ToString() const;
};

class Gamepad
{
public:
    Gamepad() {}
    virtual ~Gamepad();

    // Возвращаем вектор указателей
    vector<GamepadInfo*>& GetGamepads() { return m_gamepads; }

    void Initialize();
    void Detect();
    bool Added(SDL_Event _event);
    bool Removed(SDL_Event _event);
    string ToString();

private:
    bool Add(SDL_JoystickID _joystickId);
    vector<GamepadInfo*> m_gamepads;
};

#endif // GAMEPAD_H