
#ifndef COMPATABILITY_H
#define COMPATABILITY_H

#ifdef _MSC_VER // other possibilities are WIN32 _WIN32 or _WIN64

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <memory>
#include <functional>
#include <direct.h>
using std::static_pointer_cast;
using std::bind;
using namespace std::placeholders;

#define MY_RESOURCES_FOLDER "\code"

#define initGame initGameWindows
#define CHDIR _chdir

void initGameWindows();


#define SDL_JOY_BUTTON_A  0
#define SDL_JOY_BUTTON_B  1
#define SDL_JOY_BUTTON_X 2
#define SDL_JOY_BUTTON_Y  3
#define SDL_JOY_BUMB_L  4
#define SDL_JOY_BUMB_R  5
#define SDL_JOY_BACK  6
#define SDL_JOY_START  7
#define SDL_JOY_BUTTON_XBOX  8
#define SDL_JOY_STICK_L_CLICK  9
#define SDL_JOY_STICK_R_CLICK  10
#define SDL_JOY_TRIGGER_L  2
#define SDL_JOY_TRIGGER_R  5
#define SDL_JOY_DPAD_L  2
#define SDL_JOY_STICK_L_X  0
#define SDL_JOY_STICK_L_Y  1
#define SDL_JOY_STICK_R_X  4
#define SDL_JOY_STICK_R_Y  5


#else

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"
#include "SDL_mixer/SDL_mixer.h"
#include "SDL_image/SDL_image.h"
#include <tr1/memory>
#include <tr1/functional>
using std::tr1::static_pointer_cast;
using std::tr1::bind;
using namespace std::tr1::placeholders;

#include "main.h"
//#ifndef MY_APPLICATION_NAME
//# define MY_APPLICATION_NAME "Sid_Game_Master.app"
//#endif
#define MY_RESOURCES_FOLDER MY_APPLICATION_NAME "/Contents/Resources"

#define initGame initGameMac
#define CHDIR chdir

void initGameMac();


#define SDL_JOY_STICK_L_X  0
#define SDL_JOY_STICK_L_Y  1
#define SDL_JOY_TRIGGER_L  4
#define SDL_JOY_TRIGGER_R  5
#define SDL_JOY_BUTTON_XBOX  10
#define SDL_JOY_BUTTON_A  11
#define SDL_JOY_BUTTON_B  12
#define SDL_JOY_BUTTON_X 13
#define SDL_JOY_BUTTON_Y  14
#define SDL_JOY_START  4
#define SDL_JOY_BACK  5
#define SDL_JOY_STICK_L_CLICK  6
#define SDL_JOY_STICK_R_CLICK  7
#define SDL_JOY_BUMB_L  8
#define SDL_JOY_BUMB_R  9
#define SDL_JOY_DPAD_L  2


#endif












#endif /* COMPATABILITY_H */
