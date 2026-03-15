#include "Gamepad.h"

// --- Методы структуры GamepadInfo ---

SDL_GamepadButton GamepadInfo::ButtonDown(SDL_Event _event) const
{
    // Проверяем, что событие — нажатие кнопки и что оно принадлежит ЭТОМУ геймпаду
    if ((_event.type != SDL_EVENT_GAMEPAD_BUTTON_DOWN) || (_event.gdevice.which != ID))
    {
        return SDL_GAMEPAD_BUTTON_INVALID;
    }
    return (SDL_GamepadButton)_event.gbutton.button;
}

SDL_GamepadButton GamepadInfo::ButtonUp(SDL_Event _event) const
{
    // Проверяем, что событие — отпускание кнопки и что оно принадлежит ЭТОМУ геймпаду
    if ((_event.type != SDL_EVENT_GAMEPAD_BUTTON_UP) || (_event.gdevice.which != ID))
    {
        return SDL_GAMEPAD_BUTTON_INVALID;
    }
    return (SDL_GamepadButton)_event.gbutton.button;
}

bool GamepadInfo::ButtonHeld(SDL_GamepadButton _button) const
{
    // Опрос состояния конкретной кнопки в реальном времени
    return SDL_GetGamepadButton(Gamepad, _button);
}

string GamepadInfo::ToString() const
{
    string s = "Buttons Held: ";
    // Проходим по всем возможным кнопкам геймпада
    for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
    {
        if (SDL_GetGamepadButton(Gamepad, (SDL_GamepadButton)i))
        {
            s += to_string(i) + " ";
        }
    }
    return s;
}

// --- Методы класса Gamepad ---

void Gamepad::Initialize()
{
    M_ASSERT(SDL_InitSubSystem(SDL_INIT_GAMEPAD), "Failed to initialize gamepad input.");
}

void Gamepad::Detect()
{
    if (!SDL_HasGamepad()) return;

    int count = 0;
    // В SDL3 получаем список ID всех подключенных геймпадов
    SDL_JoystickID* ids = SDL_GetGamepads(&count);

    // Iterate over the list of gamepads
    for (int i = 0; i < count; i++)
    {
        Add(ids[i]);
    }
    SDL_free(ids); // В SDL3 нужно освобождать список ID
}

bool Gamepad::Added(SDL_Event _event)
{
    if (_event.type != SDL_EVENT_GAMEPAD_ADDED) return false;
    return Add(_event.gdevice.which);
}

bool Gamepad::Removed(SDL_Event _event)
{
    if (_event.type != SDL_EVENT_GAMEPAD_REMOVED) return false;

    for (unsigned int i = 0; i < m_gamepads.size(); i++)
    {
        if (m_gamepads[i].ID == _event.gdevice.which)
        {
            // Закрываем устройство перед удалением из вектора
            SDL_CloseGamepad(m_gamepads[i].Gamepad);
            m_gamepads.erase(m_gamepads.begin() + i);
            return true;
        }
    }
    return false;
}

bool Gamepad::Add(SDL_JoystickID _joystickId)
{
    // Проверяем, нет ли уже такого геймпада в нашем списке
    for (const auto& gp : m_gamepads)
    {
        if (gp.ID == _joystickId) return false;
    }

    // Пытаемся открыть геймпад
    SDL_Gamepad* gamepad = SDL_OpenGamepad(_joystickId);
    if (gamepad == nullptr) return false;

    GamepadInfo gpInfo;
    gpInfo.ID = _joystickId;
    gpInfo.Gamepad = gamepad;
    gpInfo.Name = SDL_GetGamepadName(gamepad);

    m_gamepads.push_back(gpInfo);
    return true;
}

string Gamepad::ToString()
{
    if (m_gamepads.empty()) return "No Gamepads Connected";

    int gpc = 0;
    string s = "";
    for (const auto& gp : m_gamepads)
    {
        gpc++;
        s += to_string(gpc) + " - " + gp.Name + ": \n";
        s += "   " + gp.ToString() + "\n";
    }
    return s;
}