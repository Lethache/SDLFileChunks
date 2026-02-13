#ifndef RENDERER_H
#define RENDERER_H

#include "StandartIncludes.h"
#include "Texture.h"

class Asset;


class Renderer : public Singleton<Renderer>
{
public:
    // Constructors/Destructors
    Renderer();
    virtual ~Renderer();

    // Accessors
    SDL_Window* GetWindow() { return m_window; }
    SDL_Renderer* GetRenderer() { return m_renderer; }
    SDL_Texture* GetSDLTexture(Texture* _texture);

    // Methods
    void Initialize(int _xResolution, int _yResolution);
    SDL_Point GetWindowSize();
    void SetDrawColor(SDL_Color _color);
    void ClearScreen();
    void SetViewport(SDL_Rect _rect);


    void RenderPoint(SDL_FPoint _position);
    void RenderLine(SDL_FRect _lineRect);
    void RenderRectangle(SDL_FRect _rect);
    void RenderFillRectangle(SDL_FRect _rect);

    void RenderTexture(Texture* _texture, SDL_Point _point);
    void RenderTexture(Texture* _texture, SDL_FRect _rect);
    void RenderTexture(Texture* _texture, SDL_FRect _srcRect, SDL_FRect _destRect);
	
    void Shutdown();
private:
    // Members
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_FRect m_destRect;

    SDL_Surface* m_surface;
    map<string, SDL_Texture*> m_textures;
};


#endif // RENDERER_H