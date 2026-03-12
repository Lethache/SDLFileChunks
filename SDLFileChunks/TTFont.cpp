#include "TTFont.h"
#include "Renderer.h"

TTFont::TTFont()
{
    m_font = nullptr;
    destRect = {};
}

TTFont::~TTFont()
{
    Shutdown();
}

void TTFont::Initialize(int _pointSize)
{
    
    M_ASSERT((TTF_Init()), "Unable to initialize SDL TTF.");
    m_font = TTF_OpenFont("../Assets/Fonts/arial.ttf", (float)_pointSize);
    M_ASSERT((m_font != nullptr), "Failed to load font.");
}

void TTFont::Shutdown()
{
    if (m_font)
    {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
    TTF_Quit();
}

void TTFont::Write(SDL_Renderer* _renderer, const char* _text, SDL_Color _color, SDL_Point _pos)
{
    if (strlen(_text) == 0) return;

    SDL_Surface* surface = TTF_RenderText_Blended(m_font, _text, 0, _color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);

   
    SDL_FRect fDestRect{ (float)_pos.x, (float)_pos.y, (float)surface->w, (float)surface->h };

 
    M_ASSERT((SDL_RenderTextureRotated(_renderer, texture, nullptr, &fDestRect, 0, nullptr, SDL_FLIP_NONE)),
        "Could not render texture");

    SDL_DestroySurface(surface); // В SDL3 это DestroySurface
    SDL_DestroyTexture(texture);
}