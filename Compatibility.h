
#ifndef COMPATABILITY_H
#define COMPATABILITY_H

void initGUI();

#ifdef _MSC_VER // other possibilities are WIN32 _WIN32 or _WIN64

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <memory>
#include <functional>
#include <direct.h>

#elif __APPLE__ // Mac OSX

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"
#include "SDL_mixer/SDL_mixer.h"
#include "SDL_image/SDL_image.h"
#include <memory>
#include <functional>


#else // Linux?

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"
#include "SDL_mixer/SDL_mixer.h"
#include "SDL_image/SDL_image.h"
#include <memory>
#include <functional>


#endif












#endif /* COMPATABILITY_H */
