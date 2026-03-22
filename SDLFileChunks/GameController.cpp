#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"      
#include "Mouse.h"         
#include "Gamepad.h"      
#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"
#include "WAVDraw.h"
#include "AssetController.h"
#include "Asset.h"

GameController::GameController() : m_quit(false), m_zoomY(1.0f)
{
    m_renderer = nullptr;
    m_fArial20 = nullptr;
    m_input = nullptr;
    m_audio = nullptr;
    m_effect = nullptr;
    m_song = nullptr;
    m_wavDraw = nullptr;
}

GameController::~GameController() { ShutDown(); }

void GameController::Initialize()
{
    AssetController::Instance().Initialize(10000000);
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();

    m_input = &InputController::Instance();
    m_input->Initialize(m_renderer->GetWindow());

    m_fArial20 = new TTFont();
    m_fArial20->Initialize(20);

    m_audio = &AudioController::Instance();
    m_wavDraw = new WAVDraw();

    m_effect = m_audio->LoadEffect("../Assets/Audio/Effects/Whoosh.wav");
    m_song = m_audio->LoadSong("../Assets/Audio/Music/Track1.mp3");
}

void GameController::ShutDown()
{
    if (m_fArial20) { delete m_fArial20; m_fArial20 = nullptr; }
    if (m_wavDraw) { delete m_wavDraw; m_wavDraw = nullptr; }
}

void GameController::HandleInput(SDL_Event _event)
{
    m_input->Process();

    
    if (_event.type == SDL_EVENT_QUIT || m_input->KB()->KeyUp(_event, SDLK_ESCAPE))
    {
        m_quit = true;
    }

   
    if (m_input->KB()->KeyUp(_event, SDLK_P)) m_audio->Play(m_effect);

   
    if (m_input->KB()->KeyUp(_event, SDLK_A)) m_zoomY += 0.2f;
    if (m_input->KB()->KeyUp(_event, SDLK_S)) m_zoomY -= 0.2f;
}

void GameController::RunGame()
{
    Initialize();
    while (!m_quit)
    {
        m_renderer->SetDrawColor({ 255, 255, 255, 255 });
        m_renderer->ClearScreen();

        while (SDL_PollEvent(&m_sdlEvent)) { HandleInput(m_sdlEvent); }

        
        if (m_effect) m_wavDraw->DrawWave(m_effect->GetData(), m_renderer, m_zoomY);

        
        if (m_audio->GetMusic())
        {
            string musInfo = "Music: " + m_audio->GetMusicTitle() + " [" + m_audio->MusicPosition() + "/" + m_audio->GetMusicLength() + "]";
            m_fArial20->Write(m_renderer->GetRenderer(), musInfo.c_str(), { 0, 0, 255 }, { 50, 50 });
        }

        
        int count = 0;
        for (auto const& e : m_audio->GetEffects())
        {
            string effInfo = "Effect " + to_string(count) + ": " + e->m_name;
            m_fArial20->Write(m_renderer->GetRenderer(), effInfo.c_str(), { 255, 0, 0 }, { 50, 100 + count * 30 });
            count++;
        }

        SDL_RenderPresent(m_renderer->GetRenderer());
    }
}