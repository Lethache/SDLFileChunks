#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"

GameController::GameController()
{
    m_sdlEvent = { };
    m_renderer = nullptr;
    m_fArial20 = nullptr;
    m_input = nullptr;
    m_quit = false;
}

GameController::~GameController()
{
    ShutDown();
}

void GameController::Initialize()
{
  
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();

    
    m_input = &InputController::Instance();
    m_input->Initialize(m_renderer->GetWindow());

    
    m_fArial20 = new TTFont();
    m_fArial20->Initialize(20);
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
    string temp;

    
    if ((_event.type == SDL_EVENT_QUIT) ||
        (m_input->KB()->KeyUp(_event, SDLK_ESCAPE)))
    {
        m_quit = true;
    }
   
    else if ((temp = m_input->KB()->TextInput(_event)) != "")
    {
        m_inputText += temp;
    }
    
    else if (m_input->KB()->KeyUp(_event, SDLK_RETURN))
    {
        m_inputText = "";
    }

   
    m_keyStates = "Current Keys Down: ";
    if (m_input->KB()->GetKeyStates()[SDL_SCANCODE_W])
    {
        m_keyStates += "W ";
    }
    if (m_input->KB()->GetKeyStates()[SDL_SCANCODE_S])
    {
        m_keyStates += "S ";
    }
}

void GameController::RunGame()
{
    Initialize();

    while (!m_quit)
    {
       
        m_renderer->SetDrawColor({ 255, 255, 255, 255 });
        m_renderer->ClearScreen();

        
        while (SDL_PollEvent(&m_sdlEvent))
        {
            HandleInput(m_sdlEvent);
        }

        m_fArial20->Write(m_renderer->GetRenderer(), m_inputText.c_str(),
            SDL_Color{ 0, 255, 0, 255 }, SDL_Point{ 250, 200 });

        m_fArial20->Write(m_renderer->GetRenderer(), m_keyStates.c_str(),
            SDL_Color{ 0, 255, 0, 255 }, SDL_Point{ 250, 220 });

        
        SDL_RenderPresent(m_renderer->GetRenderer());
    }
}