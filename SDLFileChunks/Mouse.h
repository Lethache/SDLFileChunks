#ifndef MOUSE_H
#define MOUSE_H

#include "StandartIncludes.h"

class Mouse {
public:
    Mouse() {}

   
    void Process();

  
    void ProcessButtons(SDL_Event _event);
};

#endif