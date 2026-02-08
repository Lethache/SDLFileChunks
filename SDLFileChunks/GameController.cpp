#include "GameController.h"
#include "Renderer.h"
#include "AssetController.h"
#include "Texture.h"
#include "ObjectPool.h"

GameController::GameController()
{
    m_sdlEvent = {};
}

GameController::~GameController()
{
}

void GameController::RunGame()
{
    AssetController::Instance().Initialize(100000000);

    Renderer* r = &Renderer::Instance();
    r->Initialize(800, 600);

    Texture::Pool = new ObjectPool<Texture>();
    Texture* texture = Texture::Pool->GetResource();
    texture->Load("../Assets/Textures/Emoji.tga");

    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&m_sdlEvent))
        {
            if (m_sdlEvent.type == SDL_EVENT_QUIT)
                running = false;
        }

        // Красный фон как на слайде
        r->SetDrawColor(SDL_Color{ 255, 0, 0, 255 });
        r->ClearScreen();

        r->RenderTexture(texture, SDL_Point{ 10, 10 });

        SDL_RenderPresent(r->GetRenderer());
    }

    delete Texture::Pool;
    r->Shutdown();
}
