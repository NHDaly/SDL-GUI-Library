//
//  GUIApp.cpp
//  Deep
//
//  Created by Nathan Daly on 11/27/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "GUIApp.h"
#include "GUIView.h"
#include "GUIWindow.h"

#include "GUIUtility.h"

#include "SDL/SDL.h"

#include <iostream>
#include <functional>
using std::cout; using std::endl;
using std::list;
using std::bind;
using namespace std::placeholders;

#define FPS_CAP_DEFAULT 40

using GUI::call_error_handlers;

// SINGLETON MEMBERS

GUIApp* GUIApp::singleton_ptr = 0;
GUIApp::GUIApp_destroyer GUIApp::the_GUIApp_destroyer;

GUIApp* GUIApp::get(){
	
	if (!singleton_ptr)
		singleton_ptr = new GUIApp;
	return singleton_ptr;
}

GUIApp::GUIApp_destroyer::~GUIApp_destroyer(){
	delete GUIApp::singleton_ptr;
}

// Forward Declarations
void print_msg(const GUIError &e);
void unhandled_click(const Unhandled_Click &e);


// GUIApp Implementation:

void print_msg(const GUIError &e) {
    cout << e.msg << endl;
}
void unhandled_click(const Unhandled_Click &e) {
    cout << "unhandled click" << endl;
}

struct GUIApp_Quitter {
    GUIApp_Quitter(bool &running_) : running(running_) {}
    void operator()(GUIQuit) { running = false; }
    bool &running;
};


GUIApp::GUIApp()
:window(0), fps_cap(FPS_CAP_DEFAULT), cap_frame_rate(true)
{ }

void GUIApp::cancel_timer_op(GUITimer_command* op) {
    std::vector<GUITimer_command*>::iterator it
    = std::find(timer_commands.begin(), timer_commands.end(), op);
    if (it != timer_commands.end()) {
        timer_commands.erase(it);
    }
    else {
        throw GUIError("command not found!");
    }
}

DispPoint GUIApp::get_screen_size() { return window->get_dim(); }


void GUIApp::run(GUIWindow* window_) {
    
    window = window_;
    
    register_exception_handler<GUIError>(&print_msg);

    bool running = true;

    register_exception_handler<GUIQuit>(GUIApp_Quitter(running));
    
    window->refresh();
    
    
    if (timer_commands.size())
        next_timer_cmd = timer_commands.begin();
    
    while(running) {
        SDL_Event event;
        
        try {
            FrameRateCapper capper(fps_cap);
            
            if (cap_frame_rate)
            capper.cap_frame_rate();

            
            while (SDL_PollEvent(&event) && running){

                switch (event.type) {
                        
                    case SDL_MOUSEBUTTONDOWN: 
                    case SDL_MOUSEBUTTONUP:
                    case SDL_MOUSEMOTION: {
                        
                        // Send mouse event to correct view.

                        DispPoint click_pos(event.button.x, event.button.y);
                        DispPoint rel_pos(event.motion.xrel, event.motion.yrel);
                        
                        list<GUIController*> focus_copy(captured_focus.begin(), captured_focus.end());
                        
                        for (list<GUIController*>::iterator it = focus_copy.begin();
                                            it != focus_copy.end(); ++it) {
                            
                            GUIController *captured = *it;
                            
                            DispPoint new_pos(click_pos);

                            // If the Controller is a view, adjust pos for view.
                            if (GUIView *view = dynamic_cast<GUIView*>(captured)) {
                                new_pos.x -= view->get_abs_pos().x; 
                                new_pos.y -= view->get_abs_pos().y; 
                            }
                            
                            bool handled;
                            
                            cout << "event.button.button -- " << (int)event.button.button << endl;
                            if (event.button.button == SDL_BUTTON_X1) {
                                cout << "SIDEWAYS SCROLL!" << endl;
                            }
                            if (event.button.button == SDL_BUTTON_WHEELUP) {
                                if (event.button.type == SDL_MOUSEBUTTONDOWN) {
                                    handled = captured->handle_mouse_scroll_start(true);
                                }
                                else if (event.button.type == SDL_MOUSEBUTTONUP) {
                                    handled = captured->handle_mouse_scroll_stop(true);
                                }
                            }
                            else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
                                if (event.button.type == SDL_MOUSEBUTTONDOWN) {
                                    handled = captured->handle_mouse_scroll_start(false);
                                }
                                else if (event.button.type == SDL_MOUSEBUTTONUP) {
                                    handled = captured->handle_mouse_scroll_stop(false);
                                }
                            }
                            else {
                                if (event.button.type == SDL_MOUSEBUTTONDOWN) {
                                    handled = captured->handle_mouse_down(new_pos);
                                }
                                else if (event.button.type == SDL_MOUSEBUTTONUP) {
                                    handled = captured->handle_mouse_up(new_pos);
                                }
                                else if (event.button.type == SDL_MOUSEMOTION) {
                                    handled = captured->handle_mouse_motion(new_pos, rel_pos);
                                }
                            }
//                            if (!handled) {
//                                //...
//                            }
                        }
                        
                        GUIView* hovered_view =
                        window->get_main_view()->get_view_from_point(click_pos);
                        

                        if (hovered_view) {
                            DispPoint new_pos(click_pos);

                            new_pos.x -= hovered_view->get_abs_pos().x; 
                            new_pos.y -= hovered_view->get_abs_pos().y; 
                          
                            if (event.button.button == SDL_BUTTON_WHEELUP) {
                                if (event.button.type == SDL_MOUSEBUTTONDOWN) {
                                    hovered_view->mouse_scroll_start(true);
                                }
                                else if (event.button.type == SDL_MOUSEBUTTONUP) {
                                    hovered_view->mouse_scroll_stop(true);
                                }
                            }
                            else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
                                if (event.button.type == SDL_MOUSEBUTTONDOWN) {
                                    hovered_view->mouse_scroll_start(false);
                                }
                                else if (event.button.type == SDL_MOUSEBUTTONUP) {
                                    hovered_view->mouse_scroll_stop(false);
                                }
                            }
                            else {
                                
                                if (event.button.type == SDL_MOUSEBUTTONDOWN) {
                                    hovered_view->mouse_down(new_pos);
                                }
                                else if (event.button.type == SDL_MOUSEBUTTONUP) {
                                    hovered_view->mouse_up(new_pos);
                                }  
                                else if (event.button.type == SDL_MOUSEMOTION) {
                                    hovered_view->mouse_motion(new_pos, rel_pos);
                                }
                            }
                        } 
                        
                        break;
                    }
                    case SDL_KEYDOWN: {
                        cout << "KEYDOWN" << endl;
                        
                        
                        for (view_list_t::iterator it = captured_focus.begin();
                             it != captured_focus.end(); ++it) {
                            
                            GUIController *captured = *it;
                            
                            bool handled = captured->handle_key_down(event.key.keysym);
                            if (!handled) {}
                        }

                        break;
                    }
                    case SDL_KEYUP: {
                        
                        cout << "KEYUP" << endl;

                        // Quit Key
                        if (event.key.keysym.sym == SDLK_q){
                            
#ifdef _MSC_VER  // Windows
                            if (event.key.keysym.mod == KMOD_LCTRL
								|| event.key.keysym.mod == KMOD_RCTRL
								|| event.key.keysym.mod == KMOD_CTRL){
                                throw GUIQuit();
							}
#else           // Mac
							if (event.key.keysym.mod == KMOD_LMETA
								|| event.key.keysym.mod == KMOD_RMETA
								|| event.key.keysym.mod == KMOD_META){
                                throw GUIQuit();
							}
#endif
						}
                        
                        for (view_list_t::iterator it = captured_focus.begin();
                             it != captured_focus.end(); ++it) {
                            
                            GUIController *captured = *it;
                        
                            bool handled = captured->handle_key_up(event.key.keysym);
                            if (!handled) {}
                    
                        }
                        break;
                    }
   
                    case SDL_QUIT:
						throw GUIQuit();
						break;
                        
					default:
						break;
                        
                }
            }

            cycle_timer_commands();            
        }
        
        catch(...) {
            
            call_error_handlers(handler_list.begin(), handler_list.end());
            
        }
       
        window->refresh();
        
    }
}

void GUIApp::cycle_timer_commands() {
    
    for (size_t i = 0; i < timer_commands.size(); i++) {
        
        GUITimer_command *cmd = *next_timer_cmd;
        
        if (++next_timer_cmd == timer_commands.end()) {
            next_timer_cmd = timer_commands.begin();
        }
        
        cmd->execute_command();
    }
    
}




