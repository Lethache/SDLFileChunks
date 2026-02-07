#ifndef RENDERER_H
#define RENDERER_H

#include "StandartIncludes.h"

class Renderer : public Singleton<Renderer>
{
public:
    // Constructors/Destructors
    Renderer();
    virtual ~Renderer();

    // Accessors
    SDL_Window* GetWindow() { return m_window; }
    SDL_Renderer* GetRenderer() { return m_renderer; }

    // Methods
    void Initialize(int _xResolution, int _yResolution);
    void SetDrawColor(SDL_Color _color);
    void ClearScreen();
    void Shutdown();

private:
    // Members
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Rect m_destRect;
};

#endif // RENDERER_H
