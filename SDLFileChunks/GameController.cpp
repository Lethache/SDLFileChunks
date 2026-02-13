#include "GameController.h"
#include "Renderer.h"
#include "ObjectPool.h"   
#include "Texture.h" 
#include "AssetController.h"
#include "SpriteSheet.h"
GameController::GameController()
{
    m_sdlEvent = { };
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

    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();

    SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("../Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_IDLE, 0, 6, 0.01f);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 0.005f);

    ofstream writeStream("resource.bin", ios::out | ios::binary);
    sheet->Serialize(writeStream);
    writeStream.close();

    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;
    AssetController::Instance().Clear();
    AssetController::Instance().Initialize(10000000); // Allocate 10MB
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();


    SpriteSheet* sheet2 = SpriteSheet::Pool->GetResource();
    ifstream readStream("resource.bin", ios::in | ios::binary);
    sheet2->Deserialize(readStream);
    readStream.close();

    m_sdlEvent = {};
    while (m_sdlEvent.type != SDL_EVENT_QUIT)
    {
        SDL_PollEvent(&m_sdlEvent);

        r->SetDrawColor(SDL_Color{ 255, 255, 255, 255 });
        r->ClearScreen();

        r->RenderTexture(sheet2,
            sheet2->Update(EN_AN_IDLE),
            SDL_FRect{ 0.0f, 0.0f, 69.0f * 3.0f, 44.0f * 3.0f });

        r->RenderTexture(sheet2,
            sheet2->Update(EN_AN_RUN),
            SDL_FRect{ 0.0f, 150.0f, 69.0f * 3.0f, 44.0f * 3.0f });

        SDL_RenderPresent(r->GetRenderer());
    }

    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;

    r->Shutdown();
}
