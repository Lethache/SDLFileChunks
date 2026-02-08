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

    void RenderPoint(SDL_FPoint _position);
    void RenderLine(SDL_FRect _lineRect);
    void RenderRectangle(SDL_FRect _rect);
    void RenderFillRectangle(SDL_FRect _rect);

    void RenderLine(float x1, float y1, float x2, float y2);

private:
    // Members
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Rect m_destRect;
};

#endif // RENDERER_H
