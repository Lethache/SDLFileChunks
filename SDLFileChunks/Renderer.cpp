#include "Renderer.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

Renderer::Renderer()
{
    m_window = nullptr;
    m_renderer = nullptr;
    m_destRect = { };
}

Renderer::~Renderer()
{
    Shutdown();
}

void Renderer::Initialize(int _xResolution, int _yResolution)
{
    M_ASSERT((SDL_InitSubSystem(SDL_INIT_VIDEO) >= 0), "Failed to initialize SDL video.");
    SDL_CreateWindowAndRenderer("My SDL3 Game", _xResolution, _yResolution, 0, &m_window, &m_renderer);
    M_ASSERT(m_window != nullptr, "Failed to initialize SDL window.");
    M_ASSERT(m_renderer != nullptr, "Failed to initialize SDL renderer.");
}

void Renderer::Shutdown()
{
    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit(); //Quit SDL subsystems
}

void Renderer::SetDrawColor(SDL_Color _color)
{
    SDL_SetRenderDrawColor(m_renderer, _color.r, _color.g, _color.b, _color.a);
}

void Renderer::ClearScreen()
{
    SDL_RenderClear(m_renderer);
}

void Renderer::RenderPoint(SDL_FPoint _position)
{
    SDL_RenderPoint(m_renderer, _position.x, _position.y);
}

void Renderer::RenderLine(SDL_FRect _points)
{
    SDL_RenderLine(m_renderer, _points.x, _points.y, _points.x + _points.w, _points.y + _points.h);
}

void Renderer::RenderRectangle(SDL_FRect _rect)
{
    SDL_RenderRect(m_renderer, &_rect);
}

void Renderer::RenderFillRectangle(SDL_FRect _rect)
{
    SDL_RenderFillRect(m_renderer, &_rect);
}
void Renderer::RenderLine(float x1, float y1, float x2, float y2)
{
    SDL_RenderLine(m_renderer, x1, y1, x2, y2);
}