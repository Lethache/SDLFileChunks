// GameController.cpp (вставка как на слайде)
#include "GameController.h"
#include "SpriteSheet.h"
#include "Renderer.h"
#include "TTFont.h"
#include "AssetController.h"
#include "SpriteAnim.h"
#include "ObjectPool.h"
#include "GameController.h"

GameController::GameController()
{
    m_sdlEvent = {};
}

GameController::~GameController()
{
}

void GameController::RunGame()
{
    AssetController::Instance().Initialize(10000000); // Allocate 10MB

    Renderer* r = &Renderer::Instance();
    r->Initialize(800, 600);
    SDL_Point ws = r->GetWindowSize();

    TTFont* font = new TTFont();
    font->Initialize(20);

    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("../Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_IDLE, 0, 6, 0.01f);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 0.005f);

    while (m_sdlEvent.type != SDL_EVENT_QUIT)
    {
        SDL_PollEvent(&m_sdlEvent);

        r->SetDrawColor(SDL_Color{ 255, 255, 255, 255 });
        r->ClearScreen();

        r->RenderTexture(sheet, sheet->Update(EN_AN_IDLE), SDL_FRect{ 0, 0, 69 * 3, 44 * 3 });
        r->RenderTexture(sheet, sheet->Update(EN_AN_RUN), SDL_FRect{ 0, 150, 69 * 3, 44 * 3 });

        std::string s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_IDLE));
        font->Write(r->GetRenderer(), s.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 50 });

        s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_RUN));
        font->Write(r->GetRenderer(), s.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });

        SDL_RenderPresent(r->GetRenderer());
    }


    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;

    font->Shutdown();
    r->Shutdown();
}
