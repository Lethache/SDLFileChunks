#ifndef STANDARD_INCLUDES_H
#define STANDARD_INCLUDES_H

#define SDL_MAIN_HANDLED
#define NATIVE_XRES 1920
#define NATIVE_YRES 1080

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cinttypes>
#include <stdio.h>
#include <thread>
#include <map>
#include <algorithm>
#include <sstream>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_mixer.h>

#include "Singleton.h"

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


typedef SDL_Point Point;

#endif // STANDARD_INCLUDES_H