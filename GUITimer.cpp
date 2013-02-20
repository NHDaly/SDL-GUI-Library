    


#include "GUITimer.h"




void GUITimer::start() { 
	if(!running){ 
		running = true;
		ticks_at_start = SDL_GetTicks();
	}
}
void GUITimer::stop() {
	running = false;
}
void GUITimer::pause() {
	paused = true;
	ticks_at_pause = SDL_GetTicks() - ticks_at_start;
}
void GUITimer::unpause() {
	paused = false;
}

void GUITimer::reset(){
	stop();
	start();
}

Uint32 GUITimer::get_time(){
	if (!running) {
		return 0;
	}
	if (paused) {
		return ticks_at_pause;
	}
	else {
		return SDL_GetTicks() - ticks_at_start;
	}
}


FrameRateCapper::FrameRateCapper(int frames_per_second_cap)
:fps_cap (frames_per_second_cap), capping(false)
{ }

void FrameRateCapper::cap_frame_rate(){
	capping = true;
	fps.start();
}
FrameRateCapper::~FrameRateCapper(){
	if (!capping) return;
	
	if (fps.get_time() < (Uint32) (1000 / fps_cap)) {
		SDL_Delay((1000 / fps_cap) - fps.get_time());
	}
}


