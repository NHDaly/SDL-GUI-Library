
#ifndef GUI_AUDIO_H
#define GUI_AUDIO_H

#include <string>

#include "Compatibility.h"


namespace GUI {


class Music {
public:
    Music(std::string filename);
    ~Music();
    
    operator Mix_Music*() { return music_impl; }
private:
    Mix_Music* music_impl;
};


class Sound_clip {
public:
    Sound_clip(std::string filename);
    ~Sound_clip();
    
    operator Mix_Chunk*() { return sound_clip_impl; }
    
    void play(int vol = 10, int delay = 0, int pan= 0, int pitch = 0);
    
    //private:
    Mix_Chunk* sound_clip_impl;
};



void initAudio(int flags);



} // namespace GUI




#endif /* GUI_AUDIO_H */


