
#include "GUIWin_Ctrl.h"
#include "GUIWindow.h"
#include "GUIView_Ctrl.h"
#include "GUIText_box.h"

#include "GameDisplay.h"
#include "GUIAudio.h"

#include <algorithm>
#include <cstdarg>
using std::cout; using std::endl;
using std::for_each;
using std::tr1::dynamic_pointer_cast;
using std::tr1::static_pointer_cast;




// SINGLETON MEMBERS

GUIWin_Ctrl* GUIWin_Ctrl::singleton_ptr = 0;
GUIWin_Ctrl_destroyer the_GUIWin_Ctrl_destroyer;

GUIWin_Ctrl* GUIWin_Ctrl::get(){
	
	if (!singleton_ptr)
		singleton_ptr = new GUIWin_Ctrl( new GUIWindow(SCREEN_WIDTH,SCREEN_HEIGHT));
	return singleton_ptr;
}

GUIWin_Ctrl_destroyer::~GUIWin_Ctrl_destroyer(){
	delete GUIWin_Ctrl::singleton_ptr;
}



template <typename T>
void try_catch(const T&, void(*handler)(const ErrorType&));



GUIWin_Ctrl::GUIWin_Ctrl(GUIWindow* window_)
:window(window_)
{
	
}
GUIWin_Ctrl::~GUIWin_Ctrl(){ 
	cout << "destructing GUIWin_Ctrl " << endl;
	for (std::vector<GUITimer_command*>::iterator i = timer_commands.begin(); i != timer_commands.end(); ++i)
	{
		delete *i;
	}
}


void GUIWin_Ctrl::run(int fps_cap){
	
	SDL_Event event;
	
	bool cap_frame_rate = 1;
	
	bool running = true;
	while (running) {
		try {
			FrameRateCapper capper(fps_cap);
			
			if (cap_frame_rate)
				capper.cap_frame_rate();
			
			
			//		while (SDL_PollEvent(&event)){
			//			switch (event.type) {
			//				case SDL_KEYDOWN:{
			//					switch (event.key.keysym.sym){
			//						case SDLK_UP:
			//						case SDLK_DOWN:
			//						case SDLK_LEFT:
			//						case SDLK_RIGHT:{
			//							if (event_responders.empty()) {
			//								cout << "no responder" << endl;
			//								break;
			//							}
			//							event_responders[0]->key_down(event);
			//							break;
			//						}
			//							//						case SDLK_W:
			//							//						case SDLK_S:
			//							//						case SDLK_A:
			//							//						case SDLK_D:{
			//							//							if (!event_responders[1]) {
			//							//								cout << "no responder" << endl;
			//							//								break;
			//							//							}
			//							//							event_responders[1]->key_down(event);
			//							//							break;
			//							//						}
			//					}
			//				}
			while (SDL_PollEvent(&event) && running){
				switch (event.type) {
					case SDL_KEYDOWN:{
						if (event_responders.empty()) {
							cout << "no responder" << endl;
							break;
						}
						for_each(event_responders.begin(), event_responders.end(), bind(&GUIView_Ctrl::key_down, _1, event));
						//					event_responder->key_down(event);
						break;
					}
						
					case SDL_KEYUP:{
                        if (event.key.keysym.sym == SDLK_q){
                            
							if (event.key.keysym.mod == KMOD_LMETA
								|| event.key.keysym.mod == KMOD_RMETA
								|| event.key.keysym.mod == KMOD_META){
								running = 0;
							}
						}
						break;
                        if (event_responders.empty()) {
							cout << "no responder" << endl;
							break;
						}
						for_each(event_responders.begin(), event_responders.end(), bind(&GUIView_Ctrl::key_up, _1, event));
						break;
					}
						
					case SDL_MOUSEBUTTONUP: /// @todo Are these exactly the same?
					case SDL_MOUSEBUTTONDOWN:{
						
						
						if (window->grid[event.button.y][event.button.x].empty()) {
							break;
						}
						GUIWindow::GUIView_Box_shptr_t clicked_view = window->grid[event.button.y][event.button.x].front();
						
						SDL_Event click = event;
						click.button.x -= clicked_view->pos.x; 
						click.button.y -= clicked_view->pos.y; 
						clicked_view->view->mouse_click(click);
						
						GUIVC_shptr_t view_ctrl = dynamic_pointer_cast<GUIView_Ctrl>(clicked_view->view);
						cout << view_ctrl << endl;
						if (view_ctrl){
							add_responder(view_ctrl);
						}
						
						if (event_responders.empty()) {
//							cout << "no responder" << endl;
							break;
						}
						for_each(event_responders.begin(), event_responders.end(), bind(&GUIView_Ctrl::mouse_click_down, _1, event));
						//					event_responder->mouse_click_down(event);
						//						break;
						
						
						break;
					}
//					case SDL_MOUSEBUTTONUP:
//						if (event_responders.empty()) {
////							cout << "no responder" << endl;
//							break;
//						}
//						for_each(event_responders.begin(), event_responders.end(), bind(&GUIView_Ctrl::mouse_click_up, _1, event));
//						//					event_responder->mouse_click_up(event);
//						break;
					case SDL_MOUSEMOTION:
						if (event_responders.empty()) {
							break;
						}
						for_each(event_responders.begin(), event_responders.end(), bind(&GUIView_Ctrl::mouse_motion, _1, event));
						//					event_responder->mouse_motion(event);
						break;
						
					case SDL_JOYAXISMOTION:
					case SDL_JOYBALLMOTION:
					case SDL_JOYHATMOTION:
					case SDL_JOYBUTTONDOWN:
					case SDL_JOYBUTTONUP:
						if (event_responders.empty()) {
							cout << "no responder" << endl;
							break;
						}
						for_each(event_responders.begin(), event_responders.end(), bind(&GUIView_Ctrl::joystick_event, _1, event));
						//					event_responder->mouse_motion(event);
						break;
						
					case SDL_QUIT:
						running = 0;
						break;
						
					default:
						break;
				}
			}
			
			for_each(timer_commands.begin(), timer_commands.end(), bind(&GUITimer_command::execute_command, _1));
			window->display();
		}
		catch(const Error& e){
			cout << e.msg << endl;
		}
//		catch(...){
//            
//            for (ErrorsList_t::iterator it = errors_to_handlers.begin(); 
//                    it != errors_to_handlers.end(); ++it) {
//                
//                try_catch(*it->error, it->handler);
//            }
//            
//		}
	}
}

template <typename T>
void try_catch(const T&, void(*handler)(const ErrorType&)) {
    
    try {
        throw;
    }
    catch (const T& e) {
        ((void(*)(const T&))handler)(e);
        
    }
    
}


DispPoint GUIWin_Ctrl::get_mouse_offsets(GUIView_shptr_t view){
	return window->get_view_box(view)->pos;
}

void GUIWin_Ctrl::display_on_screen(const GUIImage& image, DispPoint pos){
	window->display_on_screen(image, pos);
}

void GUIWin_Ctrl::attach(GUIVC_shptr_t view_ctrl){
	
	
}
void GUIWin_Ctrl::detach(GUIVC_shptr_t view_ctrl){
	
	
}


