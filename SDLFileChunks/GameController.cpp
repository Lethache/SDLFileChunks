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
    m_gpInfo = "";
}

GameController::~GameController()
{
    ShutDown();
}

void GameController::Initialize()
{
    // Получаем экземпляр рендерера и инициализируем его
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();

    // Получаем экземпляр контроллера ввода и инициализируем его
    m_input = &InputController::Instance();
    m_input->Initialize(m_renderer->GetWindow());

    // Создаем и инициализируем шрифт (путь должен соответствовать твоей папке Assets)
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
    // Обновляем состояние мыши в каждом кадре
    m_input->Process();

    // 1. Проверка на выход из игры
    if ((_event.type == SDL_EVENT_QUIT) ||
        (m_input->KB()->KeyUp(_event, SDLK_ESCAPE)))
    {
        m_quit = true;
    }

    // 2. Логика геймпада: обновление при подключении/отключении
    if ((m_input->GP()->Added(_event)) || (m_input->GP()->Removed(_event)))
    {
        m_gpInfo = m_input->GP()->ToString();
    }

    // 3. Логика геймпада: обновление при нажатии любой кнопки
    for (const auto& gp : m_input->GP()->GetGamepads())
    {
        if ((gp.ButtonDown(_event) != SDL_GAMEPAD_BUTTON_INVALID) ||
            (gp.ButtonUp(_event) != SDL_GAMEPAD_BUTTON_INVALID))
        {
            m_gpInfo = m_input->GP()->ToString();
        }
    }

    // 4. Логика мыши: собираем состояния кнопок
    m_keyStates = "Mouse: ";
    if (m_input->MS()->LDown()) m_keyStates += "L ";
    if (m_input->MS()->RDown()) m_keyStates += "R ";
    if (m_input->MS()->MDown()) m_keyStates += "M ";

    // 5. Логика мыши: получаем координаты курсора
    m_inputText = "Pos: " + std::to_string((int)m_input->MS()->GetX()) +
        ", " + std::to_string((int)m_input->MS()->GetY());
}

void GameController::RunGame()
{
    Initialize();

    // Первичный поиск подключенных геймпадов при старте
    m_input->GP()->Detect();
    m_gpInfo = m_input->GP()->ToString();

    while (!m_quit)
    {
        // Очистка экрана (белый фон)
        m_renderer->SetDrawColor({ 255, 255, 255, 255 });
        m_renderer->ClearScreen();

        // Цикл обработки событий
        while (SDL_PollEvent(&m_sdlEvent))
        {
            HandleInput(m_sdlEvent);
        }

        // --- Рендеринг текста ---

        // Состояние кнопок мыши (зеленый)
        m_fArial20->Write(m_renderer->GetRenderer(), m_keyStates.c_str(),
            SDL_Color{ 0, 200, 0, 255 }, SDL_Point{ 50, 50 });

        // Координаты мыши (синий)
        m_fArial20->Write(m_renderer->GetRenderer(), m_inputText.c_str(),
            SDL_Color{ 0, 0, 200, 255 }, SDL_Point{ 50, 80 });

        // Информация о геймпаде (красный)
        m_fArial20->Write(m_renderer->GetRenderer(), m_gpInfo.c_str(),
            SDL_Color{ 200, 0, 0, 255 }, SDL_Point{ 50, 120 });

        // Вывод кадра на экран
        SDL_RenderPresent(m_renderer->GetRenderer());
    }
}