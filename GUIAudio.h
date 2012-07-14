
#ifndef GUI_AUDIO_H
#define GUI_AUDIO_H

#include <string>

#include "Compatibility.h"


class GUIMusic {
public:
	GUIMusic(std::string filename);
	~GUIMusic();
	
	operator Mix_Music*() { return music_impl; }
private:
	Mix_Music* music_impl;
};


class GUISound_clip {
public:
	GUISound_clip(std::string filename);
	~GUISound_clip();
	
	operator Mix_Chunk*() { return sound_clip_impl; }
	
	void play(int vol = 10, int delay = 0, int pan= 0, int pitch = 0);

//private:
	Mix_Chunk* sound_clip_impl;
};



void initAudio(int flags);







#endif /* GUI_AUDIO_H */


