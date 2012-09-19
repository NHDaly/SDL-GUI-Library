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
        
        delete children.front();
        children.pop_front();
    }
    
    SDL_FreeSurface(image);
    if (background) delete background;
}

void NewGUIView::draw_onto_self(NewGUIView* view, DispPoint pos) {
    
    if (parent) parent->mark_changed();
    
    // Render the image onto self.
    display_image_on_self(view->image, view->w, view->h, pos);
}


// Displays on image onto this.
void NewGUIView::display_image_on_self(SDL_Surface* source, int w, int h, DispPoint pos) {
    
    // Using SDL, perform a blit from view to self.
	SDL_Rect dest_rect = {pos.x, pos.y, w, h};
	SDL_BlitSurface(source, 0, image, &dest_rect);
}

bool x_then_y_view_less_than(const NewGUIView* a, const NewGUIView* b) {
    if (a->pos.x < b->pos.x) return true;
    else if (a->pos.x == b->pos.x) return (a->pos.y < b->pos.y);
    else /*(a->pos.x > b->pos.x)*/ return false;
}

void NewGUIView::mark_changed() {
    
    changed = true;
    
    if (parent) parent->mark_changed();
    
    if (children.size() > 1) {
        children.sort(x_then_y_view_less_than);
    }
}


void NewGUIView::refresh() {
    
    if (!need_to_refresh()) return;
    
    // Refresh self. (First display background, then each child.)
    if (background) display_image_on_self(background->image, background->w, background->h, DispPoint(0,0));
    
    Subview_list_t::iterator child;
    for(child = children.begin(); child != children.end(); child++) {
        
        (*child)->refresh();
        draw_onto_self(*child, (*child)->pos);
    }
    
    changed = false;
}

void NewGUIView::attach_subview(NewGUIView* view, DispPoint pos) {
    if (view->parent)
        throw Error("Candidate vew is already a subview of another view.");
    
    /// @todo Check if out of bounds?
    
    view->pos = pos;
    children.push_back(view);
    view->parent = this;
    
    mark_changed();
}
// NOTE: Does not delete the view, only remove it from list!
void NewGUIView::remove_subview(NewGUIView* view) {
    children.remove(view); // Dummy Subview.
    
    mark_changed();
}
void NewGUIView::move_subview(NewGUIView* view, DispPoint pos) {
    NewGUIView* child = *find(children.begin(), children.end(), view);
    child->pos = pos;
    
    mark_changed();
}

#include "NewGUIWindow.h" // For Unhandled Click.

void NewGUIView::mouse_click(DispPoint coord) {
    if (!handle_mouse_down(coord)) {
        if (parent) parent->mouse_click(coord);
        else throw Unhandled_Click(coord);
    }
}

NewGUIView* NewGUIView::get_view_from_point(DispPoint coord) {
    
    if (!rel_point_is_on_me(coord)) return 0;
    
    // At worst, we know the pint is on this view.
    NewGUIView* result = this;
    
    // Check if any children have a deeper subview:
    Subview_list_t::iterator child;
    for (child = children.begin(); child != children.end(); ++child) {
        
        // Can assume that Views are sorted, so any new best will be above old best.
        NewGUIView* new_best = (*child)->get_view_from_point(adjust_to_rel(coord));
        if (new_best) {
            result = new_best;
        }
    }
    
    return result;
}

bool NewGUIView::rel_point_is_on_me(DispPoint coord) {
    
    return (coord.x >= pos.x && coord.y >= pos.y
            && coord.x <= pos.x + w && coord.y <= pos.y + h);
}
bool NewGUIView::abs_point_is_on_me(DispPoint coord) {
    
    DispPoint abs_pos = get_abs_pos();
    return (coord.x >= abs_pos.x && coord.y >= abs_pos.y
            && coord.x <= abs_pos.x + w && coord.y <= abs_pos.y + h);
}


DispPoint NewGUIView::abs_from_rel(DispPoint coord) {
    
    DispPoint abs_pos = get_abs_pos();
        return DispPoint(abs_pos.x + coord.x,
                         abs_pos.y + coord.y);
}
DispPoint NewGUIView::adjust_to_rel(DispPoint coord) {
    return DispPoint(coord.x - pos.x, coord.y - pos.y);
}


DispPoint NewGUIView::get_abs_pos() {
    if (parent == 0) return pos;
    else {
        DispPoint parent_abs_pos = parent->get_abs_pos();
        return DispPoint(parent_abs_pos.x + pos.x,
                         parent_abs_pos.y + pos.y);
    }
}
DispPoint NewGUIView::get_rel_pos() {
    return pos;
}



