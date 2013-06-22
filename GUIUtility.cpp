

#include "GUIUtility.h"
#include "GUIAudio.h"

#include "Compatibility.h"


static void quit_SDL();


void initSDL(unsigned int flags){
    if ( SDL_Init(flags) < 0 ) {
        throw GUIError("Unable to init SDL: \n" + std::string(SDL_GetError()));
    }

	if(flags & SDL_INIT_AUDIO){
        GUI::initAudio(flags);
	}

	TTF_Init();
	SDL_EnableUNICODE( SDL_ENABLE );

    atexit(quit_SDL);
}

#include <iostream>
static void quit_SDL() {
    std::cout << "Thanks for playing!" << std::endl;
    
//    SDL_Quit();   // Turned off for now because it causes an issue with screen flashing..
}
