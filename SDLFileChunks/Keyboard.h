#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "StandartIncludes.h"

class Keyboard
{
public:
    // Constructors/Destructors
    Keyboard();
    virtual ~Keyboard();

    // Accessors
    const bool* GetKeyStates() const { return SDL_GetKeyboardState(nullptr); }

    // Methods
    void Initialize(SDL_Window* _window);
    bool KeyDown(SDL_Event _event, SDL_Keycode _key);
    bool KeyUp(SDL_Event _event, SDL_Keycode _key);
    string TextInput(SDL_Event _event);

private:
    const bool* m_keyStates;
};

#endif // KEYBOARD_H