#include "Gamepad.h"

void Gamepad::Initialize()
{
    M_ASSERT(SDL_InitSubSystem(SDL_INIT_GAMEPAD), "Failed to initialize gamepad input.");
}

void Gamepad::Detect()
{
    if (!SDL_HasGamepad())
    {
        return;
    }

    int count = 0;
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
        if (m_gamepads[i].ID != _event.gdevice.which) continue;

        SDL_CloseGamepad(m_gamepads[i].Gamepad);
        m_gamepads.erase(m_gamepads.begin() + i);
        break;
    }
    return true;
}

bool Gamepad::Add(SDL_JoystickID _joystickId)
{
    for (const auto& gamepadInfo : m_gamepads)
    {
        if (gamepadInfo.ID == _joystickId) return false;
    }

    SDL_Gamepad* gamepad = SDL_OpenGamepad(_joystickId);
    M_ASSERT(gamepad != nullptr, "SDL_OpenGamepad failed.");

    GamepadInfo gpInfo;
    gpInfo.ID = _joystickId;
    gpInfo.Gamepad = gamepad;
    gpInfo.Name = SDL_GetGamepadName(gamepad);

    m_gamepads.push_back(gpInfo);
    return true;
}

string Gamepad::ToString()
{
    int gpc = 0;
    string s = "Gamepads: ";
    for (GamepadInfo gp : m_gamepads)
    {
        gpc++;
        s += to_string(gpc) + " - " + gp.Name + ": ";
    }
    return s;
}