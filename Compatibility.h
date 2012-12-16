
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

#define MY_APPLICATION_NAME "Coin Checkers 2.app"
#define MY_RESOURCES_FOLDER MY_APPLICATION_NAME "/Contents/Resources"

#define initGame initGameMac
#define CHDIR chdir

void initGameMac();


#endif












#endif /* COMPATABILITY_H */
