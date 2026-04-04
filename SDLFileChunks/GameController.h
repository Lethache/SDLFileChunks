#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandartIncludes.h"

class Renderer;
class TTFont;
class InputController;
class Timing;
class PhysicsController;

class GameController : public Singleton<GameController>
{
public:
    // Constructors/Destructors
    GameController();
    virtual ~GameController();

    // Methods
    void RunGame();
    void Initialize();
    void HandleInput(SDL_Event _event);
    void ShutDown();

private:
    // Members
    SDL_Event m_sdlEvent;
    Renderer* m_renderer;
    TTFont* m_Arial20;
    bool m_quit;
    InputController* m_input;
    Timing* m_timing;
    PhysicsController* m_physics;
};

#endif // GAME_CONTROLLER_H