//
//  NewGUIApp.cpp
//  Deep
//
//  Created by Nathan Daly on 11/27/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "NewGUIApp.h"
#include "NewGUIView.h"
#include "NewGUIWindow.h"

#include "GUIUtility.h"

#include "SDL/SDL.h"

#include <iostream>
#include <tr1/functional>
using std::cout; using std::endl;
using std::list;
using std::tr1::bind;


// SINGLETON MEMBERS

NewGUIApp* NewGUIApp::singleton_ptr = 0;
NewGUIApp::NewGUIApp_destroyer NewGUIApp::the_NewGUIApp_destroyer;

NewGUIApp* NewGUIApp::get(){
	
	if (!singleton_ptr)
		singleton_ptr = new NewGUIApp;
	return singleton_ptr;
}

NewGUIApp::NewGUIApp_destroyer::~NewGUIApp_destroyer(){
	delete NewGUIApp::singleton_ptr;
}

// Forward Declarations
void print_msg(const Error &e);
void unhandled_click(const Unhandled_Click &e);


// NewGUIApp Implementation:

void print_msg(const Error &e) {
    cout << e.msg << endl;
}
void unhandled_click(const Unhandled_Click &e) {
    cout << "unhandled click" << endl;
}

struct NewGUIApp_Quitter {
    NewGUIApp_Quitter(bool &running_) : running(running_) {}
    void operator()(GUIQuit) { running = false; }
    bool &running;
};

void NewGUIApp::run(NewGUIWindow* window) {
    
    register_error_handler<Error>(&print_msg);
    register_error_handler<Unhandled_Click>(&unhandled_click);

    bool running = true;

    register_error_handler<GUIQuit>(NewGUIApp_Quitter(running));
    
    window->refresh();
    
    while(running) {
        SDL_Event event;
        
        try {
            
            while (SDL_PollEvent(&event) && running){
                
                switch (event.type) {
                        
                    case SDL_MOUSEBUTTONDOWN: 
                    case SDL_MOUSEBUTTONUP:
                    case SDL_MOUSEMOTION: {
                        
                        // Send mouse event to correct view.

                        DispPoint click_pos(event.button.x, event.button.y);
                        DispPoint rel_pos(event.motion.xrel, event.motion.yrel);
                        
                        list<NewGUIView*> focus_copy(captured_focus.begin(), captured_focus.end());
                        
                        for (list<NewGUIView*>::iterator it = focus_copy.begin();
                                            it != focus_copy.end(); ++it) {
                            
                            NewGUIView *captured = *it;
                            
                            DispPoint new_pos(click_pos);

                            new_pos.x -= captured->get_abs_pos().x; 
                            new_pos.y -= captured->get_abs_pos().y; 
                            
                            bool handled;
                            if (event.button.type == SDL_MOUSEBUTTONDOWN) {
                                handled = captured->handle_mouse_down(new_pos);
                            }
                            else if (event.button.type == SDL_MOUSEBUTTONUP) {
                                handled = captured->handle_mouse_up(new_pos);
                            }
                            else if (event.button.type == SDL_MOUSEMOTION) {
                                handled = captured->handle_mouse_motion(new_pos, rel_pos);
                            }
//                            if (handled) {
//                                break;
//                            }
                        }
                        
                        NewGUIView* clicked_view =
                        window->get_main_view()->get_view_from_point(click_pos);
                        

                        if (clicked_view) {
                            DispPoint new_pos(click_pos);

                            new_pos.x -= clicked_view->get_abs_pos().x; 
                            new_pos.y -= clicked_view->get_abs_pos().y; 
                            
                            if (event.button.type == SDL_MOUSEBUTTONDOWN) {
                                clicked_view->mouse_down(new_pos);
                            }
                            else if (event.button.type == SDL_MOUSEBUTTONUP) {
                                clicked_view->mouse_up(new_pos);
                            }  
                            else if (event.button.type == SDL_MOUSEMOTION) {
                                clicked_view->mouse_motion(new_pos, rel_pos);
                            }

                        } 
                        
                        break;
                    }
                    case SDL_KEYDOWN: {
                        cout << "KEYDOWN" << endl;
                        
                        
                        for (view_list_t::iterator it = captured_focus.begin();
                             it != captured_focus.end(); ++it) {
                            
                            NewGUIView *captured = *it;
                            
                            bool handled = captured->handle_key_down(event.key.keysym);
                            
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
                            
                            NewGUIView *captured = *it;
                        
                            bool handled = captured->handle_key_up(event.key.keysym);
                        
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

            for_each(timer_commands.begin(), timer_commands.end(), bind(&GUITimer_command::execute_command, _1));
            
        }

        catch(...) {
            
            call_error_handlers(handler_list.begin(), handler_list.end());
            
        }
       
        window->refresh();
        
    }
}





