#ifndef MOUSE_H
#define MOUSE_H

#include "StandartIncludes.h"

class Mouse
{
public:
    // Constructors/Destructors
    Mouse();
    virtual ~Mouse() {}

    // Accessors
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    bool LDown() const { return (m_buttonStates & SDL_BUTTON_LMASK) != 0; }
    bool RDown() const { return (m_buttonStates & SDL_BUTTON_RMASK) != 0; }
    bool MDown() const { return (m_buttonStates & SDL_BUTTON_MMASK) != 0; }

    // Methods
    void Process();

private:
    float m_x;
    float m_y;
    SDL_MouseButtonFlags m_buttonStates;
};

#endif // MOUSE_H