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
                    case SDL_MOUSEBUTTONUP:
                    case SDL_MOUSEMOTION: {
                        
                        // Send mouse event to correct view.

                        DispPoint click_pos(event.button.x, event.button.y);
                        DispPoint rel_pos(event.motion.xrel, event.motion.yrel);
                        
                        if (captured) {
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
                            if (handled) {
                                break;
                            }
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
