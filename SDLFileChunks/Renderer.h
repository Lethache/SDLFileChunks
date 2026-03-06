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
    vector<SDL_DisplayMode>& GetResolutions() { return m_resolutions; }

    // Methods
    void Initialize();
    void ChangeDisplayMode(SDL_DisplayMode* _mode, bool _fullscreen);
    SDL_Point GetPrimaryResolution();
    void EnumerateDisplayModes();
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
    void RenderTexture(SDL_Texture* _texture, SDL_FRect _srcRect, SDL_FRect _destRect, double _angle);
    void Shutdown();

private:
    // Members
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_FRect m_destRect;
    SDL_Surface* m_surface;
    map<string, SDL_Texture*> m_textures;
    vector<SDL_DisplayMode> m_resolutions;
};

#endif // RENDERER_H