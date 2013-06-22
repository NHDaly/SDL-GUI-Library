//
//  GUIWindow.cpp
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "GUIWindow.h"
#include "GameDisplay.h"
#include "GUIUtility.h"
#include "GUIView.h"
#include "Compatibility.h"

#include SDL_SDL_H
#include SDL_SDLVIDEO_H
#include SDL_SDLKEYBOARD_H

#include <iostream>
#include <algorithm>
using namespace std;


namespace GUI {

bool Window::WINDOW_ALREADY_CREATED = false;

SDL_Color bg_color = {208, 208, 208};

Window::Window(int w, int h, const string& name_, 
                           int numColors, const Uint32 flags)
:main_view(0), name(name_), num_colors(numColors), flags(flags)
{    
    /// @TODO HACK!
    if (WINDOW_ALREADY_CREATED) throw Error("Sorry, already created a window!");

    SDL_Color *colors = new SDL_Color[numColors];
	// Create a display surface with a 16 bit palette 
	window = createDisplay(window, w, h, 16, colors, numColors, flags);
	
	delete colors;

	if (!window) throw Error("Couldn't Create Window.");
	
	//Set the window caption 
    if (name != "") {
        SDL_WM_SetCaption( name.c_str(), 0 );
    }
    
    main_view = new View(w,h);
    main_view->mark_changed();
    
   	Uint32 color = SDL_MapRGB(main_view->image->format, bg_color.r, 
                              bg_color.g, bg_color.b);
    SDL_FillRect(main_view->image, 0, color);
    
    refresh();
    
    WINDOW_ALREADY_CREATED = true;
    
}

DispPoint Window::get_dim() { return DispPoint(window->w, window->h); }


void Window::resize(int w, int h) {
    
//    SDL_free(window);
    WINDOW_ALREADY_CREATED = false;
    
    Window temp(w,h, name, num_colors, flags);
    
    std::swap(temp.window, window);    
    main_view->resize(w, h);
}
void Window::stretch(int w, int h) {
    
}

void Window::rename(const string& name_) {
    name = name_;
}

#include <cassert>
void Window::refresh() {
    
    if (main_view->need_to_refresh()) {
        
        main_view->refresh();

        // Using SDL, perform a blit from main_view to self.
        SDL_Rect dest_rect = {0,0, main_view->w, main_view->h};
        SDL_BlitSurface(main_view->display, 0, window, &dest_rect);

        updateScreen(window);
        
//        static int loop = 1;
//        if (loop++ % 10) { 
//            return;
//        }
//        
//        char buffer[5999999];
//        SDL_RWops *ops = SDL_RWFromMem(buffer, sizeof(buffer));
//        int succ = SDL_SaveBMP_RW(main_view->image, ops, 0);
//        assert(succ == 0);
//        
////        cout << "ops->hidden.mem.base" << (long)ops->hidden.mem.base << endl;
////        cout << "ops->hidden.mem.here" << (long)ops->hidden.mem.here << endl;
////        cout << "ops->hidden.mem.stop" << (long)ops->hidden.mem.stop << endl;
//        int len = ops->hidden.mem.stop - ops->hidden.mem.base;
////        cout << "len\n " << (int)len << endl;
//
//        len = ops->seek(ops, 0,SEEK_CUR);
//        cout << "len\n " << (int)len << endl;
//
//        main_view->image = SDL_LoadBMP_RW(SDL_RWFromMem(&buffer, 5999999), 1);
//        if(!main_view) {
//            cout << SDL_GetError() << endl;
//            assert(0);
//        }
//        SDL_BlitSurface(main_view->display, 0, window, &dest_rect);
//        updateScreen(window);
//   
//        SDL_FreeRW(ops);
    }
}


void Window::attach_subview(View* view, DispPoint pos) {
    main_view->attach_subview(view, pos);
}
void Window::move_subview(View* view, DispPoint pos) {
    main_view->move_subview(view, pos);
}
void Window::remove_subview(View* view) {
    main_view->remove_subview(view); 
}
void Window::remove_last_subview() {
    main_view->remove_last_subview();
}

} // namespace GUI
