// TTFont.cpp
#include "TTFont.h"
#include "Renderer.h"

TTFont::TTFont()
{
    m_font = nullptr;
    destRect = {};
}

void TTFont::Initialize(int _pointSize)
{
    M_ASSERT((TTF_Init() >= 0), "Unable to initialize SDL TTF.");
    M_ASSERT((m_font = TTF_OpenFont("../Assets/Fonts/arial.ttf", _pointSize)) != nullptr, "Failed to load font.");
}

void TTFont::Shutdown()
{
    TTF_Quit();
}

void TTFont::Write(SDL_Renderer* _renderer, const char* _text, SDL_Color _color, SDL_Point _pos)
{
    SDL_Surface* surface;
    surface = TTF_RenderText_Blended(m_font, _text, strlen(_text), _color);

    SDL_Texture* texture;
    texture = SDL_CreateTextureFromSurface(_renderer, surface);

    SDL_FRect destRect{ _pos.x, _pos.y, surface->w, surface->h };
    M_ASSERT((SDL_RenderTextureRotated(_renderer, texture, nullptr, &destRect, 0,
        nullptr, SDL_FLIP_NONE) >= 0), "Could not render texture");

    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}
