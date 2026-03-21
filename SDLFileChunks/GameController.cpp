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
#include "AssetController.h" // Исправляет ошибку incomplete type
#include "Asset.h"           // Исправляет ошибку с GetData()

GameController::GameController()
{
    m_sdlEvent = { };
    m_renderer = nullptr;
    m_fArial20 = nullptr;
    m_input = nullptr;
    m_quit = false;
    m_audio = nullptr;
    m_effect = nullptr;
    m_song = nullptr;
}

GameController::~GameController()
{
    ShutDown();
}

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
    m_effect = m_audio->LoadEffect("../Assets/Audio/Effects/DrinkSipSwallow.mp3");
    m_song = m_audio->LoadSong("../Assets/Audio/Music/Track1.mp3");
}

void GameController::ShutDown()
{
    if (m_fArial20 != nullptr)
    {
        delete m_fArial20;
        m_fArial20 = nullptr;
    }
}

void GameController::HandleInput(SDL_Event _event)
{
    m_input->Process();

    if ((_event.type == SDL_EVENT_QUIT) ||
        (m_input->KB()->KeyUp(_event, SDLK_ESCAPE)))
    {
        m_quit = true;
    }

    if (m_input->KB()->KeyUp(_event, SDLK_P)) m_audio->Play(m_effect);
    if (m_input->KB()->KeyUp(_event, SDLK_SPACE)) m_audio->Play(m_song);
    if (m_input->KB()->KeyUp(_event, SDLK_S)) m_audio->StopMusic();
    if (m_input->KB()->KeyUp(_event, SDLK_D)) m_audio->PauseMusic();
    if (m_input->KB()->KeyUp(_event, SDLK_F)) m_audio->ResumeMusic();
}

void GameController::RunGame()
{
    Initialize();
    m_input->GP()->Detect();

    while (!m_quit)
    {
        m_renderer->SetDrawColor({ 255, 255, 255, 255 });
        m_renderer->ClearScreen();

        while (SDL_PollEvent(&m_sdlEvent))
        {
            HandleInput(m_sdlEvent);
        }

        if (m_audio->GetMusic() != nullptr)
        {
            string musInfo = "Music: Name - " + m_audio->GetMusicTitle() +
                "  Pos - " + m_audio->MusicPosition() + "/" + m_audio->GetMusicLength();
            m_fArial20->Write(m_renderer->GetRenderer(), musInfo.c_str(), SDL_Color{ 0, 0, 255 }, { 50, 50 });
        }

        if (m_effect != nullptr && m_effect->GetData() != nullptr)
        {
            string effInfo = "Effect: Name - " + m_effect->GetData()->GetGUID();
            m_fArial20->Write(m_renderer->GetRenderer(), effInfo.c_str(), SDL_Color{ 0, 255, 0 }, { 50, 100 });
        }

        SDL_RenderPresent(m_renderer->GetRenderer());
    }
}