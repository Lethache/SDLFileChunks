#include "Renderer.h"
#include "Asset.h"

Renderer::Renderer()
{
    m_window = nullptr;
    m_renderer = nullptr;
    m_destRect = {};
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

void Renderer::SetDrawColor(SDL_Color _color)
{
    SDL_SetRenderDrawColor(m_renderer, _color.r, _color.g, _color.b, _color.a);
}

void Renderer::ClearScreen()
{
    SDL_RenderClear(m_renderer);
}

void Renderer::Shutdown()
{
    for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
        SDL_DestroyTexture(it->second);
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

SDL_Texture* Renderer::GetSDLTexture(Texture* _texture)
{
    Asset* asset = _texture->GetData();
    string guid = asset->GetGUID();

    if (m_textures.count(guid) != 0)
        return m_textures[guid];

    ImageInfo* ii = _texture->GetImageInfo();

   
    SDL_PixelFormat format = (ii->BitsPerPixel == 32) ? SDL_PIXELFORMAT_BGRA32 : SDL_PIXELFORMAT_BGR24;

    void* pixels = (void*)(asset->GetData() + ii->DataOffset);
    int pitch = ii->Width * (ii->BitsPerPixel / 8);

    m_surface = SDL_CreateSurfaceFrom(ii->Width, ii->Height, format, pixels, pitch);
    M_ASSERT(m_surface != nullptr, "Could not create surface from texture data");

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    M_ASSERT(tex != nullptr, "Could not create texture from surface");

    SDL_DestroySurface(m_surface);
    m_surface = nullptr;

    m_textures[guid] = tex;
    return tex;
}

void Renderer::RenderTexture(Texture* _texture, SDL_Point _point)
{
    m_destRect.x = (float)_point.x;
    m_destRect.y = (float)_point.y;  
    m_destRect.w = (float)_texture->GetImageInfo()->Width;
    m_destRect.h = (float)_texture->GetImageInfo()->Height;

    M_ASSERT(
        SDL_RenderTextureRotated(m_renderer, GetSDLTexture(_texture),
            NULL, &m_destRect, 0.0, NULL, SDL_FLIP_VERTICAL) >= 0,
        "Could not render texture"
    );
}
