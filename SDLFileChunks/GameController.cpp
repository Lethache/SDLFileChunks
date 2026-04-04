#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timing.h"
#include "PhysicsController.h"
#include "SpriteSheet.h"
#include "SpriteAnim.h"
#include "AssetController.h" 

GameController::GameController() {
    m_quit = false;
    m_sdlEvent = { };
    m_renderer = nullptr;
    m_fArial20 = nullptr;
    m_input = nullptr;
    m_timing = nullptr;
    m_physics = nullptr;
    m_fire = nullptr;
    m_smoke = nullptr;
}

GameController::~GameController() {
    ShutDown();
}

void GameController::Initialize() {
    AssetController::Instance().Initialize(10000000);
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();
    m_input = &InputController::Instance();

    m_fArial20 = new TTFont();
    m_fArial20->Initialize(20);

    m_timing = &Timing::Instance();
    m_physics = &PhysicsController::Instance();

    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    m_fire = SpriteSheet::Pool->GetResource();
    m_fire->Load("../Assets/Textures/Fire.tga");
    m_fire->SetSize(6, 10, 64, 64);
    m_fire->AddAnimation(EN_AN_IDLE, 0, 60, 20.0f);
    m_fire->SetBlendMode(SDL_BLENDMODE_BLEND);

    m_smoke = SpriteSheet::Pool->GetResource();
    m_smoke->Load("../Assets/Textures/Smoke.tga");
    m_smoke->SetSize(5, 6, 128, 128);
    m_smoke->AddAnimation(EN_AN_SMOKE_RISE, 0, 30, 20.0f);
    m_smoke->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void GameController::HandleInput(SDL_Event _event) {
    
    if ((_event.type == SDL_EVENT_QUIT) || (m_input->KB()->KeyUp(_event, SDLK_ESCAPE))) {
        m_quit = true;
    }
    
    else if (m_input->KB()->KeyDown(_event, SDLK_A)) {
        Particle* p = m_physics->AddParticle(glm::vec2{ 340 + rand() % 25, 230 + rand() % 10 }, 3 + rand() % 3);
        if (p) {
            p->SetBuoyancy(glm::vec2{ 0, 45 });
            p->SetBuoyancyDecay(glm::vec2{ 0, 15 });
            p->SetMass(1.0f);
            p->SetRandomForce(glm::vec2{ -15 + rand() % 30, 0 });
            p->SetWind(glm::vec2{ 5 + rand() % 5, 0 });
        }
    }
    m_input->MS()->ProcessButtons(_event);
}

void GameController::RunGame() {
    Initialize();
    while (!m_quit) {
        m_timing->Tick();
        m_renderer->SetDrawColor(SDL_Color{ 255, 255, 255, 255 });
        m_renderer->ClearScreen();

        while (SDL_PollEvent(&m_sdlEvent)) {
            HandleInput(m_sdlEvent);
        }

        m_physics->Update(m_timing->GetDeltaTime());

      
        m_renderer->RenderTexture(m_fire, m_fire->Update(EN_AN_IDLE, m_timing->GetDeltaTime()), SDL_FRect{ 300, 200, 100, 100 });

        
        SDL_FRect r = m_smoke->Update(EN_AN_SMOKE_RISE, m_timing->GetDeltaTime());
        for (Particle* p : m_physics->GetParticles()) {
            m_renderer->SetDrawColor(SDL_Color{ 0, 0, 0, 255 });
            float size = p->GetCurrentSize() * 100.0f / 2.0f;
            glm::vec2 pos = p->GetPosition();
            m_renderer->RenderTexture(m_smoke, r, SDL_FRect{ pos.x - size, pos.y - size, size, size }, (int)((1.0f - p->GetCurrentSize()) * 255));
        }

        m_fArial20->Write(m_renderer->GetRenderer(), ("FPS: " + std::to_string(m_timing->GetFPS())).c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 10, 10 });
        m_fArial20->Write(m_renderer->GetRenderer(), m_physics->ToString().c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 120, 10 });

        SDL_RenderPresent(m_renderer->GetRenderer());
    }
}

void GameController::ShutDown() {
    if (m_fArial20) { delete m_fArial20; m_fArial20 = nullptr; }
    if (SpriteAnim::Pool) { delete SpriteAnim::Pool; SpriteAnim::Pool = nullptr; }
    if (SpriteSheet::Pool) { delete SpriteSheet::Pool; SpriteSheet::Pool = nullptr; }
}