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
    texture->Load("../Assets/Textures/Warrior.tga");


   
        while (m_sdlEvent.type != SDL_EVENT_QUIT)
        {
            SDL_PollEvent(&m_sdlEvent);

            r->SetDrawColor(SDL_Color{ 255, 255, 255, 255 });
            r->ClearScreen();

            for (unsigned int count = 0; count < 6; count++)
            {
                float xPos = count * 69;

                r->RenderTexture(
                    texture,
                    SDL_FRect{ xPos, 0.0f, 69.0f, 44.0f },
                    SDL_FRect{ xPos, 100.0f, 69.0f *2 , 44.0f* 2  }
                );
            }

            SDL_RenderPresent(r->GetRenderer());
        }


        SDL_RenderPresent(r->GetRenderer());

    
    delete(Texture::Pool);
    r->Shutdown();

}