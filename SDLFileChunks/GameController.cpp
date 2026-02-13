#include "GameController.h"
#include "Renderer.h"
#include "ObjectPool.h"   
#include "Texture.h" 
#include "AssetController.h"
GameController::GameController()
{
    m_sdlEvent = { };
}

GameController::~GameController()
{
}

void GameController::RunGame()
{
    AssetController::Instance().Initialize(10000000);

    Renderer* r = &Renderer::Instance();
    r->Initialize(800, 600);
    SDL_Point ws = r->GetWindowSize();

    Texture::Pool = new ObjectPool<Texture>();
    Texture* texture = Texture::Pool->GetResource();
    texture->Load("../Assets/Textures/Emoji.tga");


    while (m_sdlEvent.type != SDL_EVENT_QUIT)
    {
        SDL_PollEvent(&m_sdlEvent);

        r->SetViewport(SDL_Rect{ 0, 0, ws.x, ws.y });
        r->SetDrawColor(SDL_Color{ 255, 0, 0, 255 });
        r->ClearScreen();
        r->SetViewport(SDL_Rect{ 0, 0, ws.x / 2, ws.y / 2 }); // Top-left
        r->RenderTexture(texture, SDL_Point{ 10, 10 });
        r->SetViewport(SDL_Rect{ ws.x / 2, 0, ws.x, ws.y / 2 }); // Top-right
        r->RenderTexture(texture, SDL_FRect{ 0.0f, 0.0f, (float)(ws.x / 2), (float)(ws.y / 2) });
        r->SetViewport(SDL_Rect{ 0, ws.y / 2, ws.x, ws.y }); // Bottom-left
        r->RenderTexture(texture, SDL_FRect{ 0.0f, 0.0f, (float)(ws.x / 2), (float)(ws.y / 2) });
        r->SetViewport(SDL_Rect{ ws.x / 2, ws.y / 2, ws.x, ws.y }); // Bottom-right
        r->RenderTexture(texture, SDL_Point{ 0, 0 });

        SDL_RenderPresent(r->GetRenderer());

    }
    delete(Texture::Pool);
    r->Shutdown();

}