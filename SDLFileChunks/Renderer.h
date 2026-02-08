#ifndef RENDERER_H
#define RENDERER_H

#include "StandartIncludes.h"
#include "Texture.h"

class Asset;

class Renderer : public Singleton<Renderer>
{
public:
    Renderer();
    virtual ~Renderer();

    SDL_Window* GetWindow() { return m_window; }
    SDL_Renderer* GetRenderer() { return m_renderer; }

    void Initialize(int _xResolution, int _yResolution);
    void SetDrawColor(SDL_Color _color);
    void ClearScreen();

    SDL_Texture* GetSDLTexture(Texture* _texture);
    void RenderTexture(Texture* _texture, SDL_Point _point);

    void Shutdown();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_FRect m_destRect;
    SDL_Surface* m_surface;
    map<string, SDL_Texture*> m_textures;
};

#endif // RENDERER_H
