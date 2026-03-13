#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "StandartIncludes.h"

class Keyboard;
class Mouse;
class Gamepad;

class InputController : public Singleton<InputController>
{
public:
    InputController();
    virtual ~InputController();

    void Initialize(SDL_Window* _window);
    void Process();

    Keyboard* KB() { return m_keyboard; }
    Mouse* MS() { return m_mouse; }
    Gamepad* GP() { return m_gamepad; }

private:
    Keyboard* m_keyboard;
    Mouse* m_mouse;
    Gamepad* m_gamepad;
};

#endif // INPUTCONTROLLER_H