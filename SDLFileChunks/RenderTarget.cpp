#include "RenderTarget.h"
#include "Renderer.h"

RenderTarget::RenderTarget()
{
    m_texture = nullptr;
    m_width = 0;
    m_height = 0;
    m_rotation = 0;
}

RenderTarget::~RenderTarget()
{
    if (m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

void RenderTarget::Create(unsigned int _width, unsigned int _height)
{
    m_width = _width;
    m_height = _height;

    m_texture = SDL_CreateTexture(Renderer::Instance().GetRenderer(), SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    M_ASSERT(m_texture != nullptr, "Unable to create render target");
}

void RenderTarget::Start()
{
    SDL_SetRenderTarget(Renderer::Instance().GetRenderer(), m_texture);
}

void RenderTarget::Stop()
{
    SDL_SetRenderTarget(Renderer::Instance().GetRenderer(), nullptr);
}

void RenderTarget::Render(float _deltaTime)
{
    SDL_Point ws = Renderer::Instance().GetWindowSize();
    float perc = float(NATIVE_YRES - ws.y) / NATIVE_YRES; // 1080 - 600 = 480 therefore 480 = 0.44444444 of 1080
    float xRes = ws.x + (NATIVE_XRES * perc); // now add 0.4444444 * NATIVE_XRES to the current resolution
    // 0.4444444 * 1920 = 853 + 800 = 1653
    float yRes = NATIVE_YRES;             // same as ws.y + (NATIVE_YRES * perc) = 600 * 0.4444444 = 480 + 600 = 1080

    Renderer::Instance().RenderTexture(m_texture,
        SDL_FRect{ 0, NATIVE_YRES - yRes, xRes, yRes },
        SDL_FRect{ 0, 0, (float)ws.x, (float)ws.y },
        m_rotation
    );
}