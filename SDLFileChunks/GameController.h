#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandartIncludes.h"

class Renderer;
class TTFont;
class InputController;
class AudioController;
class SoundEffect;
class Song;
class WAVDraw;

class GameController : public Singleton<GameController>
{
public:
    GameController();
    virtual ~GameController();

    void RunGame();
    void Initialize();
    void HandleInput(SDL_Event _event);
    void ShutDown();

private:
    SDL_Event m_sdlEvent;
    Renderer* m_renderer;
    TTFont* m_fArial20;
    bool m_quit;
    InputController* m_input;
    AudioController* m_audio;
    SoundEffect* m_effect;
    Song* m_song;
    WAVDraw* m_wavDraw;
    float m_zoomY;
};

#endif