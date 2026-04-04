#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timing.h"
#include "PhysicsController.h"
#include "AssetController.h"

GameController::GameController() {
    m_sdlEvent = { };
    m_renderer = nullptr;
    m_Arial20 = nullptr;
    m_input = nullptr;
    m_quit = false;
    m_timing = nullptr;
    m_physics = nullptr;
}

GameController::~GameController() {
    ShutDown();
}

void GameController::Initialize() {
    
    AssetController::Instance().Initialize(10000000);

    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();

    m_input = &InputController::Instance();
    m_input->Initialize(m_renderer->GetWindow());

    m_Arial20 = new TTFont();
    m_Arial20->Initialize(20);

    m_timing = &Timing::Instance();
    m_physics = &PhysicsController::Instance();
}

void GameController::HandleInput(SDL_Event _event) {
    m_input->Process();

    string temp;
    if (m_sdlEvent.type == SDL_EVENT_QUIT ||
        (m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)))
    {
        m_quit = true;
    }
    else if (m_input->KB()->GetKeyStates()[SDL_SCANCODE_A])
    {
        m_physics->AddParticle(glm::vec2{ 300 + rand() % 400, 200 }, 3 + rand() % 5);
    }
}

void GameController::RunGame() {
    Initialize();

    while (!m_quit)
    {
        m_timing->Tick();

        
        m_renderer->SetDrawColor(SDL_Color{ 255, 255, 255, 255 });
        m_renderer->ClearScreen();

        while (SDL_PollEvent(&m_sdlEvent))
        {
            HandleInput(m_sdlEvent);
        }

        m_physics->Update(m_timing->GetDeltaTime());

        
        for (Particle* p : m_physics->GetParticles())
        {
            m_renderer->SetDrawColor(SDL_Color{ 0, 0, 255, 255 });
            m_renderer->RenderFillRectangle(SDL_FRect{ p->GetPosition().x, p->GetPosition().y, 5.0f, 5.0f });
        }

        
        m_Arial20->Write(m_renderer->GetRenderer(), ("FPS: " + to_string(m_timing->GetFPS())).c_str(),
            SDL_Color{ 0, 0, 255, 255 }, SDL_Point{ 10, 10 });

        m_Arial20->Write(m_renderer->GetRenderer(), m_physics->ToString().c_str(),
            SDL_Color{ 0, 0, 255, 255 }, SDL_Point{ 120, 10 });

        SDL_RenderPresent(m_renderer->GetRenderer());
    }

}
void GameController::ShutDown() {

}