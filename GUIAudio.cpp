

#include "GUIAudio.h"

#include "GUIUtility.h"
using std::string;
#include <iostream>
using std::cout; using std::endl;



namespace GUI {


Music::Music(string filename)
{
    music_impl = Mix_LoadMUS(filename.c_str());
    if (music_impl == NULL)
        throw GUIError("Could not initialize GUIMusic with file: " + filename);
}
Music::~Music(){
    
    Mix_FreeMusic(music_impl);
}

Sound_clip::Sound_clip(string filename)
{
    sound_clip_impl = Mix_LoadWAV("audio/scratch.wav");
    if (!sound_clip_impl)
        throw GUIError("Could not initialize GUISound_clip with file: audio/scratch.wav");
}
Sound_clip::~Sound_clip(){
    
    if (sound_clip_impl)
        Mix_FreeChunk(sound_clip_impl);
}

void Sound_clip::play(int vol, int delay, int pan, int pitch){
    
    cout << "playing"<<endl;
    if( Mix_PlayChannel( -1, sound_clip_impl, 0 ) == -1 ) { 
        throw GUIError("Could not play GUISound_clip! " + std::string(SDL_GetError()));
    }
    printf("%d channels are playing\n", Mix_Playing(-1));
}


void initAudio(int flags){
    //	int initted=Mix_Init(flags);
    //	if((initted&flags) != flags) {
    //		throw GUIError("Unable to init SDL_mixer Audio: \n" + std::string(SDL_GetError()));
    //	}
	
    if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ){
        throw GUIError("Unable to init SDL_mixer Audio: \n" + std::string(SDL_GetError()));
    }
    
    atexit(Mix_Quit);
}



} // namespace GUI
