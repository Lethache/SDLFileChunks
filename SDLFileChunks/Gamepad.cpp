#include "Gamepad.h"

// --- Методы структуры GamepadInfo ---

SDL_GamepadButton GamepadInfo::ButtonDown(SDL_Event _event) const {
    if ((_event.type != SDL_EVENT_GAMEPAD_BUTTON_DOWN) || (ID != _event.gdevice.which))
        return SDL_GAMEPAD_BUTTON_INVALID;
    return (SDL_GamepadButton)_event.gbutton.button;
}

SDL_GamepadButton GamepadInfo::ButtonUp(SDL_Event _event) const {
    if ((_event.type != SDL_EVENT_GAMEPAD_BUTTON_UP) || (ID != _event.gdevice.which))
        return SDL_GAMEPAD_BUTTON_INVALID;
    return (SDL_GamepadButton)_event.gbutton.button;
}

bool GamepadInfo::ButtonHeld(SDL_GamepadButton _button) const {
    return SDL_GetGamepadButton(Gamepad, _button);
}

// Реализация GetLeftStick (которую искал линкер)
SDL_FPoint& GamepadInfo::GetLeftStick() {
    LeftStick.x = SDL_GetGamepadAxis(Gamepad, SDL_GAMEPAD_AXIS_LEFTX);
    LeftStick.y = SDL_GetGamepadAxis(Gamepad, SDL_GAMEPAD_AXIS_LEFTY);
    return LeftStick;
}

// Реализация GetRightStick
SDL_FPoint& GamepadInfo::GetRightStick() {
    RightStick.x = SDL_GetGamepadAxis(Gamepad, SDL_GAMEPAD_AXIS_RIGHTX);
    RightStick.y = SDL_GetGamepadAxis(Gamepad, SDL_GAMEPAD_AXIS_RIGHTY);
    return RightStick;
}

bool GamepadInfo::ProcessMotion(SDL_Event _event) {
    if ((_event.type != SDL_EVENT_GAMEPAD_AXIS_MOTION) || (ID != _event.cdevice.which))
        return false;

    float val = (float)_event.gaxis.value;
    if (_event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX) LeftStick.x = val;
    else if (_event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY) LeftStick.y = val;
    else if (_event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX) RightStick.x = val;
    else if (_event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY) RightStick.y = val;

    return true;
}

string GamepadInfo::ToString() const {
    string s = "Buttons: ";
    for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
        if (SDL_GetGamepadButton(Gamepad, (SDL_GamepadButton)i)) s += to_string(i) + " ";

    s += " | L: " + to_string((int)LeftStick.x) + "," + to_string((int)LeftStick.y);
    s += " | R: " + to_string((int)RightStick.x) + "," + to_string((int)RightStick.y);
    return s;
}

// --- Методы класса Gamepad ---

void Gamepad::Initialize() {
    M_ASSERT(SDL_InitSubSystem(SDL_INIT_GAMEPAD), "Failed to initialize Gamepad subsystem");
}

Gamepad::~Gamepad() {
    for (auto gp : m_gamepads) {
        if (gp->Gamepad) SDL_CloseGamepad(gp->Gamepad);
        delete gp;
    }
    m_gamepads.clear();
}

void Gamepad::Detect() {
    int count = 0;
    SDL_JoystickID* ids = SDL_GetGamepads(&count);
    if (ids) {
        for (int i = 0; i < count; i++) Add(ids[i]);
        SDL_free(ids);
    }
}

bool Gamepad::Added(SDL_Event _event) {
    if (_event.type != SDL_EVENT_GAMEPAD_ADDED) return false;
    return Add(_event.gdevice.which);
}

bool Gamepad::Removed(SDL_Event _event) {
    if (_event.type != SDL_EVENT_GAMEPAD_REMOVED) return false;
    for (int i = 0; i < m_gamepads.size(); i++) {
        if (m_gamepads[i]->ID == _event.gdevice.which) {
            SDL_CloseGamepad(m_gamepads[i]->Gamepad);
            delete m_gamepads[i];
            m_gamepads.erase(m_gamepads.begin() + i);
            return true;
        }
    }
    return false;
}

bool Gamepad::Add(SDL_JoystickID _joystickId) {
    for (auto gp : m_gamepads) if (gp->ID == _joystickId) return false;

    SDL_Gamepad* gamepad = SDL_OpenGamepad(_joystickId);
    if (!gamepad) return false;

    GamepadInfo* gpInfo = new GamepadInfo();
    gpInfo->ID = _joystickId;
    gpInfo->Gamepad = gamepad;
    gpInfo->Name = SDL_GetGamepadName(gamepad);
    m_gamepads.push_back(gpInfo);

    return true;
}

string Gamepad::ToString() {
    if (m_gamepads.empty()) return "No Gamepads Connected";
    string s = "";
    for (auto gp : m_gamepads) s += gp->Name + ": " + gp->ToString();
    return s;
}