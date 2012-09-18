//
//  NewGUIView.cpp
//  Deep
//
//  Created by Nathan Daly on 9/10/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "NewGUIView.h"

#include "SDL/SDL_video.h"


#include <iostream>
#include <algorithm> // For std::find.
using namespace std;


NewGUIView::NewGUIView(int w_, int h_) 
:changed(false), w(w_), h(h_), background(0), parent(0)
{ 
    Uint32 rmask, gmask, bmask, amask;
	
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
	
    SDL_Surface *temp = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32,
                                   rmask, gmask, bmask, amask);
    if(temp == NULL) {
		throw Error("CreateRGBSurface failed: \n" + string(SDL_GetError()));
    }
	image = SDL_DisplayFormat(temp);
	if(!image) {
        throw Error("updateimage in CreateRGBSurface failed: \n" + string(SDL_GetError()));
    }
	SDL_FreeSurface(temp);
}

NewGUIView::~NewGUIView() {
    
    while (!children.empty()) {
        
        delete children.front().view;
        children.pop_front();
    }
}

void NewGUIView::draw_onto_self(NewGUIView* view, DispPoint pos) {
    
    if (parent) parent->mark_changed();
    
    // Render the image onto self.
    display_image_on_self(view, pos);
}


// Displays on image onto this.
void NewGUIView::display_image_on_self(NewGUIView* view, DispPoint pos) {
    
    ///@todo..
    cout << "drawing " << view << " onto " << this << endl;
    
    // Using SDL, perform a blit from view to self.
	SDL_Rect dest_rect = {pos.x, pos.y, view->w, view->h};
	SDL_BlitSurface(view->image, 0, image, &dest_rect);
}

void NewGUIView::mark_changed() {
    
    changed = true;
    
    if (parent) parent->mark_changed();
}


void NewGUIView::refresh() {
    
    if (!need_to_refresh()) return;
    
    // Refresh self. (First display background, then each child.)
    if (background) display_image_on_self(background, DispPoint(0,0));
    
    Subview_list_t::iterator child;
    for(child = children.begin(); child != children.end(); child++) {
        
        child->view->refresh();
        draw_onto_self(child->view, child->pos);
    }
    
    changed = false;
}

void NewGUIView::attach_subview(NewGUIView* view, DispPoint pos) {
    if (view->parent)
        throw Error("Candidate vew is already a subview of another view.");
    
    /// @todo Check if out of bounds?
    
    children.push_back(GUISubview(view, pos));
    view->parent = this;
    
    mark_changed();
}
// NOTE: Does not delete the view, only remove it from list!
void NewGUIView::remove_subview(NewGUIView* view) {
    children.remove(GUISubview(view, DispPoint())); // Dummy Subview.
    
    mark_changed();
}
void NewGUIView::move_subview(NewGUIView* view, DispPoint pos) {
    ( find(children.begin(), children.end(),
           GUISubview(view, DispPoint())) )->pos = pos;
    
    mark_changed();
}


///@todo TEMPORARY
void NewGUIView::print_children() {
    std::cout << this << std::endl;
    
    for(Subview_list_t::iterator child = children.begin(); child != children.end(); child++) {
        std::cout << child->view << " " << child->pos.x <<" "<< child->pos.y << std::endl;
    }
}


