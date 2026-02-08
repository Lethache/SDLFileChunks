#include "GameController.h"
#include "Renderer.h"

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

    bool running = true;

    while (running)
    {
        // Handle events
        while (SDL_PollEvent(&m_sdlEvent))
        {
            if (m_sdlEvent.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // Clear screen every frame (prevents flicker/artefacts)
        r->SetDrawColor(SDL_Color{ 255, 0, 0, 255 });
        r->ClearScreen();

        // Yellow dotted line
        r->SetDrawColor(SDL_Color{ 255, 255, 0, 255 });
        for (unsigned int count = 0; count < 800; count++)
        {
            if (count % 2 == 0)
                r->RenderPoint(SDL_FPoint{ (float)count, 300 });
        }

        // Blue solid vertical line
        r->SetDrawColor(SDL_Color{ 0, 0, 255, 255 });
        r->RenderLine(SDL_FRect{ 400, 0, 1, 600 });

        // Green rectangle outline
        r->SetDrawColor(SDL_Color{ 0, 255, 0, 255 });
        r->RenderRectangle(SDL_FRect{ 200, 200, 100, 100 });

        // White filled rectangle
        r->SetDrawColor(SDL_Color{ 255, 255, 255, 255 });
        r->RenderFillRectangle(SDL_FRect{ 400, 400, 100, 100 });

        // Present
        SDL_RenderPresent(r->GetRenderer());
    }

    r->Shutdown();
}
