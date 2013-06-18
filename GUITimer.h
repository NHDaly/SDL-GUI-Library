
#ifndef TIMER_H
#define TIMER_H

#ifdef _MSC_VER // other possibilities are WIN32 _WIN32 or _WIN64
#include "SDL_timer.h"
#else
#include "SDL/SDL_timer.h"
#endif

namespace GUI {

class GUITimer {
public:
	
	GUITimer() : ticks_at_start(0), ticks_at_pause(0), running(false), paused(false) { }
	void start();
	void stop();
	void pause();
	void unpause();

	void reset();
	
	Uint32 get_time();
	
	typedef Uint32 time_t;
	
private:
	Uint32 ticks_at_start;
	Uint32 ticks_at_pause;
	bool running;
	bool paused;
};


class FrameRateCapper {
public:
	FrameRateCapper(int frames_per_second_cap);
	~FrameRateCapper();

	void cap_frame_rate();
	
private:
	GUITimer fps;
	int fps_cap;
	bool capping;
};

} // namespace GUI


#endif /* TIMER_H */




