
#ifndef COMPATABILITY_H
#define COMPATABILITY_H

void initGUI();
const char* getResourcePath(); // note user must free returned c-string

#ifdef _MSC_VER // windows

# define SDL_SDL_H "SDL.h"
# define SDL_SDLVIDEO_H "SDL_video.h"
# define SDL_SDLKEYBOARD_H "SDL_keyboard.h"
# define SDL_SDLEVENTS_H "SDL_events.h"
# define SDLIMAGE_SDLIMAGE_H "SDL_image.h"
# define SDLTTF_SDLTTF_H "SDL_ttf.h"
# define SDLMIXER_SDLMIXER_H "SDL_mixer.h"

# define TR1_FUNCTIONAL_H <functional>

#elif __APPLE__

# define SDL_SDL_H "SDL/SDL.h"
# define SDL_SDLVIDEO_H "SDL/SDL_video.h"
# define SDL_SDLKEYBOARD_H "SDL/SDL_keyboard.h"
# define SDL_SDLEVENTS_H "SDL/SDL_events.h"
# define SDLIMAGE_SDLIMAGE_H "SDL_image/SDL_image.h"
# define SDLTTF_SDLTTF_H "SDL_ttf/SDL_ttf.h"
# define SDLMIXER_SDLMIXER_H "SDL_mixer/SDL_mixer.h"

# define TR1_FUNCTIONAL_H <tr1/functional>

#else // Linux?

# define SDL_SDL_H "SDL/SDL.h"
# define SDL_SDLVIDEO_H "SDL/SDL_video.h"
# define SDL_SDLKEYBOARD_H "SDL/SDL_keyboard.h"
# define SDL_SDLEVENTS_H "SDL/SDL_events.h"
# define SDLIMAGE_SDLIMAGE_H "SDL_image/SDL_image.h"
# define SDLTTF_SDLTTF_H "SDL_ttf/SDL_ttf.h"
# define SDLMIXER_SDLMIXER_H "SDL_mixer/SDL_mixer.h"

# define TR1_FUNCTIONAL_H <tr1/functional>

#endif 		



#ifdef _MSC_VER // other possibilities are WIN32 _WIN32 or _WIN64

extern const char * MY_RESOURCES_FOLDER;

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <memory>
#include <functional>
#include <direct.h>

#elif __APPLE__ // Mac OSX

extern const char * BUNDLE_RESOURCES_FOLDER;
extern const char * STANDALONE_RESOURCES_FOLDER;

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"
#include "SDL_mixer/SDL_mixer.h"
#include "SDL_image/SDL_image.h"
#include <tr1/memory>
#include <tr1/functional>


#else // Linux?

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"
#include "SDL_mixer/SDL_mixer.h"
#include "SDL_image/SDL_image.h"
#include <tr1/memory>
#include <tr1/functional>


#endif












#endif /* COMPATABILITY_H */
