#include "Mouse.h"

Mouse::Mouse()
{
    m_x = 0;
    m_y = 0;
    m_buttonStates = 0;
}

void Mouse::Process()
{
    m_buttonStates = SDL_GetMouseState(&m_x, &m_y);
}