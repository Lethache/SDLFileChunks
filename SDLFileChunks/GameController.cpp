#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"

GameController::GameController()
{
    m_sdlEvent = { };
    m_renderer = nullptr;
    m_fArial20 = nullptr;
    m_input = nullptr;
    m_quit = false;
    m_inputText = "";
    m_keyStates = "";
    m_gpInfo = "Searching for gamepads...";
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
    
    m_input->Process();

    
    if ((_event.type == SDL_EVENT_QUIT) ||
        (m_input->KB()->KeyUp(_event, SDLK_ESCAPE)))
    {
        m_quit = true;
    }

    
    m_keyStates = "Mouse Buttons: ";
    if (m_input->MS()->LDown()) m_keyStates += "L ";
    if (m_input->MS()->RDown()) m_keyStates += "R ";
    if (m_input->MS()->MDown()) m_keyStates += "M ";

  
    m_inputText = "Mouse Pos: " + std::to_string((int)m_input->MS()->GetX()) +
        ", " + std::to_string((int)m_input->MS()->GetY());

   
    if (m_input->GP()->Added(_event) || m_input->GP()->Removed(_event))
    {
        m_gpInfo = m_input->GP()->ToString();
    }
}

void GameController::RunGame()
{
    Initialize();

   
    m_input->GP()->Detect();
   
    m_gpInfo = m_input->GP()->ToString();

    while (!m_quit)
    {
       
        m_renderer->SetDrawColor({ 255, 255, 255, 255 });
        m_renderer->ClearScreen();

        
        while (SDL_PollEvent(&m_sdlEvent))
        {
            HandleInput(m_sdlEvent);
        }

        
        SDL_Color textColor = { 0, 200, 0, 255 };

      
        m_fArial20->Write(m_renderer->GetRenderer(), m_keyStates.c_str(),
            textColor, SDL_Point{ 50, 50 });

        
        m_fArial20->Write(m_renderer->GetRenderer(), m_inputText.c_str(),
            textColor, SDL_Point{ 50, 80 });

        
        SDL_Color gpColor = { 200, 0, 0, 255 };
        m_fArial20->Write(m_renderer->GetRenderer(), m_gpInfo.c_str(),
            gpColor, SDL_Point{ 50, 120 });

        
        SDL_RenderPresent(m_renderer->GetRenderer());
    }
}