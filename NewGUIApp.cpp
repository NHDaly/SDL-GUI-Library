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
using std::cout; using std::endl;

NewGUIView *captured = 0;


void NewGUI_run(NewGUIWindow* window) {
    
    window->refresh();
    
    bool running = true;
    while(running) {
        SDL_Event event;
        
        try {
            
            while (SDL_PollEvent(&event) && running){
                
                switch (event.type) {
                    case SDL_MOUSEBUTTONDOWN: 
                    case SDL_MOUSEBUTTONUP: {
                        
                        // Send mouse click to clicked view.
                        SDL_MouseButtonEvent click_event = event.button;
                        
                        DispPoint click_pos(click_event.x, click_event.y);
                        
                        NewGUIView* clicked_view =
                        window->get_main_view()->get_view_from_point(click_pos);
                        
                        if (event.button.type == SDL_MOUSEBUTTONDOWN) {
                            if (captured) {
                                bool handled = captured->handle_mouse_down(click_pos);
                                if (handled) {
                                    break;
                                }
                            }
                            else if (clicked_view) {
                                
                                click_pos.x -= clicked_view->get_abs_pos().x; 
                                click_pos.y -= clicked_view->get_abs_pos().y; 
                                
                                clicked_view->mouse_down(click_pos);
                            }

                        }
                        else {
                            if (captured) {
                                bool handled = captured->handle_mouse_up(click_pos);
                                if (handled) {
                                    break;
                                }
                            }
                            else if (clicked_view) {
                                
                                click_pos.x -= clicked_view->get_abs_pos().x; 
                                click_pos.y -= clicked_view->get_abs_pos().y; 
                                
                                clicked_view->mouse_up(click_pos);
                            }
                        }
                        
                        break;
                    }
                    case SDL_MOUSEMOTION: {
                        
                        // Send mouse click to clicked view.
                        SDL_MouseMotionEvent motion_event = event.motion;
                        
                        DispPoint pos(motion_event.x, motion_event.y);
                        DispPoint rel(motion_event.xrel, motion_event.yrel);
                        
                        if (captured) captured->mouse_motion(pos, rel);
                        
						break;
                    }
                    case SDL_KEYUP: {
                        // Quit Key
                        if (event.key.keysym.sym == SDLK_q){
                            
							if (event.key.keysym.mod == KMOD_LMETA
								|| event.key.keysym.mod == KMOD_RMETA
								|| event.key.keysym.mod == KMOD_META){
								running = 0;
							}
						}
                        break;
                    }
                    case SDL_QUIT:
						running = 0;
						break;
					default:
						break;
                        
                }
            }
        }
        catch(const Error& e) {
            cout << e.msg << endl;
        }
        catch(const Unhandled_Click& e) {
            cout << "unhandled click" << endl;
        }
        
        window->refresh();
        
    }
}
