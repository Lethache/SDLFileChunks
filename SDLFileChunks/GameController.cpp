
#include "GameController.h"
#include "SpriteSheet.h"
#include "Renderer.h"
#include "TTFont.h"
#include "AssetController.h"
#include "SpriteAnim.h"
#include "ObjectPool.h"
#include "GameController.h"
#include "Timing.h"
#include "RenderTarget.h"
GameController::GameController()
{
    m_sdlEvent = {};
}

GameController::~GameController()
{
}
void GameController::RunGame()
{
    Renderer* r = &Renderer::Instance();
    Timing* t = &Timing::Instance();

    r->Initialize();
    r->EnumerateDisplayModes();
	r->ChangeDisplayMode(&r->GetResolutions()[1], true); // change to 1920x1080 fullscreen or another that we need 

    TTFont* font = new TTFont();
    font->Initialize(20);

    AssetController::Instance().Initialize(10000000); // Allocate 10MB
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();
    SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("../Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);

    RenderTarget* rt = new RenderTarget();
    rt->Create(NATIVE_XRES, NATIVE_YRES);
    while (m_sdlEvent.type != SDL_EVENT_QUIT)
    {
        t->Tick();
        rt->Start();
        SDL_PollEvent(&m_sdlEvent);
        r->SetDrawColor(SDL_Color{ 255, 255, 255, 255 });
        r->ClearScreen();
        r->SetDrawColor(SDL_Color{ 0, 0, 0, 255 });
        r->RenderFillRectangle(SDL_FRect{ 0, 150, 69 * 3, 44 * 3 });
        r->RenderTexture(sheet, sheet->Update(EN_AN_RUN, t->GetDeltaTime()), SDL_FRect{ 0, 150, 69 * 3, 44 * 3 });

        std::string fps = "Frames Per Second: " + std::to_string(t->GetFPS());
        font->Write(r->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });
        std::string res = "Resolution: " + std::to_string(r->GetWindowSize().x) + "x" + std::to_string(r->GetWindowSize().y);
        font->Write(r->GetRenderer(), res.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 20 });
        std::string s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_RUN));
        font->Write(r->GetRenderer(), s.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });

        rt->Stop();
        r->SetDrawColor(SDL_Color{ 0, 0, 0, 255 });
        r->ClearScreen();
        rt->Render(t->GetDeltaTime()); // Scale native resolution to screen resolution
        SDL_RenderPresent(r->GetRenderer());

        if ((m_sdlEvent.type == SDL_EVENT_KEY_DOWN) &&
            (m_sdlEvent.key.key == SDLK_ESCAPE))
        {
            break;
        }

        t->CapFPS();
    }

    delete rt;
    font->Shutdown();
    r->Shutdown();
}