#include "Renderer.h"
#include "Asset.h"

Renderer::Renderer()
{
    m_window = nullptr;
    m_renderer = nullptr;
    m_destRect = { };
    m_surface = nullptr;
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
    for (auto it = m_textures.begin(); it != m_textures.end(); it++)
    {
        SDL_DestroyTexture(it->second);
    }
    m_textures.clear();



    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit(); // Quit SDL subsystem
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





SDL_Texture* Renderer::GetSDLTexture(Texture* _texture)
{
    
    Asset* asset = _texture->GetData();
    string guid = asset->GetGUID();
    if (m_textures.count(guid) != 0)
    {
        return m_textures[guid];
    }
    
    ImageInfo* ii = _texture->GetImageInfo();

    m_surface = SDL_CreateSurfaceFrom(ii->Width, ii->Height,
        SDL_GetPixelFormatForMasks(ii->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000),
        asset->GetData() + _texture->GetImageInfo()->DataOffset, ii->Width * ii->BitsPerPixel / 8);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    SDL_DestroySurface(m_surface);
    m_surface = nullptr;
    m_textures[guid] = texture;
    return texture;
}

void Renderer::RenderTexture(Texture* _texture, SDL_Point _point)
{
    m_destRect.x = _point.x;
    m_destRect.y = _point.y;
    m_destRect.w = _texture->GetImageInfo()->Width;
    m_destRect.h = _texture->GetImageInfo()->Height;
    M_ASSERT((SDL_RenderTextureRotated(m_renderer, GetSDLTexture(_texture),
        NULL, &m_destRect, 0, NULL, SDL_FLIP_VERTICAL) >= 0), "Could not render texture");
}




SDL_Point Renderer::GetWindowSize()
{
    int w;
    int h;
    SDL_GetWindowSize(m_window, &w, &h);
    return SDL_Point{ w, h };
}

void Renderer::SetViewport(SDL_Rect _viewport)
{
    SDL_SetRenderViewport(m_renderer, &_viewport);
}

void Renderer::RenderTexture(Texture* _texture, SDL_FRect _rect)
{
    M_ASSERT((SDL_RenderTextureRotated(m_renderer, GetSDLTexture(_texture),
        NULL, &_rect, 0, NULL, SDL_FLIP_VERTICAL) >= 0), "Could not render texture");
}

void Renderer::RenderTexture(Texture* _texture, SDL_FRect _srcRect, SDL_FRect _destRect)
{
    _srcRect.y = _texture->GetImageInfo()->Height - _srcRect.y - _srcRect.h;

    M_ASSERT((SDL_RenderTextureRotated(m_renderer, GetSDLTexture(_texture),
        &_srcRect, &_destRect, 0, NULL, SDL_FLIP_VERTICAL) >= 0), "Could not render texture");
}
