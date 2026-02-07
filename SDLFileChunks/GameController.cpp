#include "GameController.h"
#include "Renderer.h"
#include "Singleton.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>

GameController::GameController()
{
    m_sdlEvent = { };
}

GameController::~GameController()
{
}

void GameController::RunGame()
{
    Renderer* r = &Renderer::Instance();
    r->Initialize(800, 600);

    while (m_sdlEvent.type != SDL_EVENT_QUIT)
    {
        SDL_PollEvent(&m_sdlEvent);
        r->SetDrawColor(SDL_Color{ 255, 0, 0, 255 });
        r->ClearScreen();
        SDL_RenderPresent(r->GetRenderer());
    }

    r->Shutdown();
}
