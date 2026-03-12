#include "Keyboard.h"

Keyboard::Keyboard()
{
    m_keyStates = nullptr;
}

Keyboard::~Keyboard()
{
}

void Keyboard::Initialize(SDL_Window* _window)
{
    SDL_StartTextInput(_window);
}

bool Keyboard::KeyDown(SDL_Event _event, SDL_Keycode _key)
{
    if (_event.type != SDL_EVENT_KEY_DOWN) return false;
    return _key == _event.key.key;
}

bool Keyboard::KeyUp(SDL_Event _event, SDL_Keycode _key)
{
    if (_event.type != SDL_EVENT_KEY_UP) return false;
    return _key == _event.key.key;
}

string Keyboard::TextInput(SDL_Event _event)
{
    if (_event.type != SDL_EVENT_TEXT_INPUT) return "";
    return string(_event.text.text);
}