#ifndef STANDARD_INCLUDES_H
#define STANDARD_INCLUDES_H


#define SDL_MAIN_HANDLER 
// Include standart headers
#include <string>
#include <vector>
#include <iostream>
#include <fstream> // Library for file operations
#include <cinttypes>
#include <stdio.h>
#include <thread>
#include <map>  
#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
// Project headers
#include "Singleton.h"

// OS specific includes and defines
#ifdef _WIN32
#include <Windows.h>
#include <direct.h>

#define M_ASSERT(_cond, _msg) \
    if (!(_cond)) { OutputDebugStringA(_msg); std::abort(); }
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;


#endif // STANDARD_INCLUDES_H 