#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"

GameController::GameController() {
    m_renderer = nullptr; m_fArial20 = nullptr; m_input = nullptr; m_quit = false;
}

GameController::~GameController() { ShutDown(); }

void GameController::Initialize() {
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();
    m_input = &InputController::Instance();
    m_input->Initialize(m_renderer->GetWindow());
    m_fArial20 = new TTFont();
    m_fArial20->Initialize(20);
}

void GameController::ShutDown() {
    if (m_fArial20) { delete m_fArial20; m_fArial20 = nullptr; }
}

void GameController::HandleInput(SDL_Event _event) {
    m_input->Process();
    if (_event.type == SDL_EVENT_QUIT || m_input->KB()->KeyUp(_event, SDLK_ESCAPE)) m_quit = true;

    if (m_input->GP()->Added(_event) || m_input->GP()->Removed(_event)) {
        m_gpInfo = m_input->GP()->ToString();
    }

    for (GamepadInfo* gp : m_input->GP()->GetGamepads()) {
        if (gp->ButtonDown(_event) != SDL_GAMEPAD_BUTTON_INVALID ||
            gp->ButtonUp(_event) != SDL_GAMEPAD_BUTTON_INVALID ||
            gp->ProcessMotion(_event)) {
            m_gpInfo = m_input->GP()->ToString();
        }
    }

    m_keyStates = "Mouse: ";
    if (m_input->MS()->LDown()) m_keyStates += "L ";
    if (m_input->MS()->RDown()) m_keyStates += "R ";

    m_inputText = "Pos: " + to_string((int)m_input->MS()->GetX()) + "," + to_string((int)m_input->MS()->GetY());
}

void GameController::RunGame() {
    Initialize();
    m_input->GP()->Detect();
    m_gpInfo = m_input->GP()->ToString();

    while (!m_quit) {
        m_renderer->SetDrawColor({ 255, 255, 255, 255 });
        m_renderer->ClearScreen();

        while (SDL_PollEvent(&m_sdlEvent)) { HandleInput(m_sdlEvent); }

        m_fArial20->Write(m_renderer->GetRenderer(), m_keyStates.c_str(), { 0, 200, 0, 255 }, { 50, 50 });
        m_fArial20->Write(m_renderer->GetRenderer(), m_inputText.c_str(), { 0, 0, 200, 255 }, { 50, 80 });
        m_fArial20->Write(m_renderer->GetRenderer(), m_gpInfo.c_str(), { 200, 0, 0, 255 }, { 50, 110 });

        SDL_RenderPresent(m_renderer->GetRenderer());
    }
}