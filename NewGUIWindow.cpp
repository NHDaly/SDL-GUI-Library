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
using namespace std;

NewGUIWindow::NewGUIWindow(NewGUIView* main_view_, const string& name_, 
                           int numColors)
:name(name_), main_view(main_view_)
{
	initialize(main_view_->image->w, main_view_->image->h,
               name_, numColors, main_view_);

}
NewGUIWindow::NewGUIWindow(int w, int h, const string& name_, 
                           int numColors)
:name(name_), main_view(0)
{    
	initialize(w, h, name_, numColors, main_view);
}
    
void NewGUIWindow::initialize(int w, int h, const std::string& name_,
                int numColors, NewGUIView* main_view_)  
{
    SDL_Color colors[numColors];
	// Create a display surface with a 16 bit palette 
	window = createDisplay(window, w, h, 16, colors, numColors);
	
	if (!window) throw Error("Couldn't Create Window.");
	
	//Set the window caption 
	SDL_WM_SetCaption( name.c_str(), 0 );

    if (main_view) {
        // Using SDL, perform a blit from main_view to self.
        SDL_Rect dest_rect = {0,0, main_view->image->w, main_view->image->h};
        SDL_BlitSurface(main_view->image, 0, window, &dest_rect);
    }
}


void NewGUIWindow::crop(int w, int h) {
    
}
void NewGUIWindow::stretch(int w, int h) {
    
}

void NewGUIWindow::rename(const string& name_) {
    name = name_;
}

void NewGUIWindow::set_main_view(NewGUIView* main_view_) { 
    if (main_view == main_view_) return;

    delete main_view;
    main_view = main_view_;
}

void NewGUIWindow::refresh() {
    
    if (main_view && main_view->need_to_refresh()) {
        
        main_view->refresh();

        // Using SDL, perform a blit from main_view to self.
        SDL_Rect dest_rect = {0,0, main_view->w, main_view->h};
        SDL_BlitSurface(main_view->display, 0, window, &dest_rect);

        updateScreen(window);
    }
}

