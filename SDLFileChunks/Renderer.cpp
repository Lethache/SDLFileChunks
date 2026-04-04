#include "Renderer.h"
#include "Asset.h"
#include <string>



Renderer::Renderer()
{
    m_window = nullptr;
    m_renderer = nullptr;
    m_surface = nullptr;
    m_destRect = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_srcRect = { 0.0f, 0.0f, 0.0f, 0.0f };
}

Renderer::~Renderer()
{
    Shutdown();
}

void Renderer::Initialize()
{
    
    M_ASSERT((SDL_InitSubSystem(SDL_INIT_VIDEO) >= 0), "Failed to initialize SDL video.");

    SDL_Point res = GetPrimaryResolution();

    
    M_ASSERT(SDL_CreateWindowAndRenderer("My SDL3 Game", res.x, res.y, 0, &m_window, &m_renderer) == true,
        "Failed to initialize SDL window and renderer.");

    M_ASSERT(m_window != nullptr, "Window is null.");
    M_ASSERT(m_renderer != nullptr, "Renderer is null.");
}

SDL_Point Renderer::GetPrimaryResolution()
{
    SDL_DisplayID primaryDisplayID = SDL_GetPrimaryDisplay();
    M_ASSERT(primaryDisplayID != 0, "Failed to get primary display.");

    const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(primaryDisplayID);
    M_ASSERT(mode != nullptr, "SDL_GetDesktopDisplayMode failed.");

    return SDL_Point{ mode->w, mode->h };
}

void Renderer::Shutdown()
{
    
    for (auto const& [guid, tex] : m_textures)
    {
        if (tex != nullptr)
        {
            SDL_DestroyTexture(tex);
        }
    }
    m_textures.clear();

    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
}

void Renderer::SetDrawColor(SDL_Color _color)
{
    SDL_SetRenderDrawColor(m_renderer, _color.r, _color.g, _color.b, _color.a);
}

void Renderer::ClearScreen()
{
    SDL_RenderClear(m_renderer);
}

void Renderer::SetViewport(SDL_Rect _viewport)
{
    SDL_SetRenderViewport(m_renderer, &_viewport);
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
    std::string guid = asset->GetGUID();

    if (m_textures.count(guid) == 0)
    {
        ImageInfo* ii = _texture->GetImageInfo();

       
        m_surface = SDL_CreateSurfaceFrom(
            ii->Width,
            ii->Height,
            SDL_GetPixelFormatForMasks(ii->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000),
            asset->GetData() + ii->DataOffset,
            ii->Width * (ii->BitsPerPixel / 8)
        );

        M_ASSERT(m_surface != nullptr, "Failed to create surface from texture data.");

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;

        m_textures[guid] = texture;
    }

   
    SDL_SetTextureBlendMode(m_textures[guid], _texture->GetBlendMode());
    SDL_SetTextureAlphaMod(m_textures[guid], _texture->GetBlendAlpha());

    return m_textures[guid];
}


void Renderer::RenderTexture(Texture* _texture, SDL_FRect _srcRect, SDL_FRect _destRect, int _alpha)
{
    m_srcRect = _srcRect;
    m_destRect = _destRect;

   
    m_srcRect.y = (float)_texture->GetImageInfo()->Height - _srcRect.y - _srcRect.h;

    SDL_Texture* tex = GetSDLTexture(_texture);
    SDL_SetTextureAlphaMod(tex, (Uint8)_alpha);

    
    M_ASSERT((SDL_RenderTextureRotated(m_renderer, tex, &m_srcRect, &m_destRect, 0.0, nullptr, SDL_FLIP_VERTICAL) >= 0),
        "Could not render texture.");
}


void Renderer::RenderTexture(Texture* _texture, SDL_FRect _rect)
{
    SDL_FRect src = { 0.0f, 0.0f, (float)_texture->GetImageInfo()->Width, (float)_texture->GetImageInfo()->Height };
    RenderTexture(_texture, src, _rect, 255);
}


void Renderer::RenderTexture(Texture* _texture, SDL_Point _point)
{
    SDL_FRect dest = { (float)_point.x, (float)_point.y, (float)_texture->GetImageInfo()->Width, (float)_texture->GetImageInfo()->Height };
    RenderTexture(_texture, dest);
}

void Renderer::RenderTexture(SDL_Texture* _texture, SDL_FRect _srcRect, SDL_FRect _destRect, double _angle)
{
    M_ASSERT((SDL_RenderTextureRotated(m_renderer, _texture, &_srcRect, &_destRect, _angle, nullptr, SDL_FLIP_NONE) >= 0),
        "Could not render SDL_Texture.");
}

SDL_Point Renderer::GetWindowSize()
{
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
    return SDL_Point{ w, h };
}

void Renderer::EnumerateDisplayModes()
{
    int numDisplays;
    SDL_DisplayID* displays = SDL_GetDisplays(&numDisplays);
    M_ASSERT(displays != nullptr, "SDL_GetDisplays failed.");

    m_resolutions.clear();

    for (int i = 0; i < numDisplays; ++i)
    {
        int numModes = 0;
        SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(displays[i], &numModes);

        if (modes != nullptr)
        {
            for (int j = 0; j < numModes; ++j)
            {
                m_resolutions.push_back(*modes[j]);
            }
            SDL_free(modes);
        }
    }
    SDL_free(displays);
}

void Renderer::ChangeDisplayMode(SDL_DisplayMode* _mode, bool _fullscreen)
{
    if (_fullscreen)
    {
        SDL_SetWindowFullscreen(m_window, true);
        SDL_SetWindowFullscreenMode(m_window, _mode);
    }
    else
    {
        SDL_SetWindowFullscreen(m_window, false);
        SDL_SetWindowSize(m_window, _mode->w, _mode->h);
    }

    SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}