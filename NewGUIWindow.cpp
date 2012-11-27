//
//  NewGUIWindow.cpp
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "NewGUIWindow.h"
#include "GameDisplay.h"
#include "GUIUtility.h"
#include "NewGUIView.h"
#include "SDL/SDL.h"

#include <iostream>
#include <algorithm>
using namespace std;

SDL_Color bg_color = {208, 208, 208};

NewGUIWindow::NewGUIWindow(int w, int h, const string& name_, 
                           int numColors, const Uint32 flags)
:main_view(0), name(name_), num_colors(numColors), flags(flags)
{    
    SDL_Color colors[numColors];
	// Create a display surface with a 16 bit palette 
	window = createDisplay(window, w, h, 16, colors, numColors, flags);
	
	if (!window) throw Error("Couldn't Create Window.");
	
	//Set the window caption 
    if (name != "") {
        SDL_WM_SetCaption( name.c_str(), 0 );
    }
    
    main_view = new NewGUIView(w,h);
    main_view->mark_changed();
    
   	Uint32 color = SDL_MapRGB(main_view->image->format, bg_color.r, 
                              bg_color.g, bg_color.b);
    SDL_FillRect(main_view->image, 0, color);
    
    refresh();
}


void NewGUIWindow::crop(int w, int h) {
    
    NewGUIWindow temp(w,h, name, num_colors, flags);
    
    std::swap(temp.window, window);    
    main_view->mark_changed();
}
void NewGUIWindow::stretch(int w, int h) {
    
}

void NewGUIWindow::rename(const string& name_) {
    name = name_;
}

void NewGUIWindow::refresh() {
    
    if (main_view->need_to_refresh()) {
        
        main_view->refresh();

        // Using SDL, perform a blit from main_view to self.
        SDL_Rect dest_rect = {0,0, main_view->w, main_view->h};
        SDL_BlitSurface(main_view->display, 0, window, &dest_rect);

        updateScreen(window);
    }
}


void NewGUIWindow::attach_subview(NewGUIView* view, DispPoint pos) {
    main_view->attach_subview(view, pos);
}
void NewGUIWindow::remove_subview(NewGUIView* view) {
    main_view->remove_subview(view); 
}
void NewGUIWindow::move_subview(NewGUIView* view, DispPoint pos) {
    main_view->move_subview(view, pos);
}


