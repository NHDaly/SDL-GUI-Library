//
//  NewGUIView.h
//  Deep
//
//  Created by Nathan Daly on 9/10/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_NewGUIView_h
#define Deep_NewGUIView_h

#include <list>

#include "GUIUtility.h"

class SDL_Surface;

class NewGUIView {
public:
    
    NewGUIView(int w_, int h_);
    ~NewGUIView();
    
    // Redraw any children that have changed onto self. Return true if changed.
    void refresh();
    
    
    // Renders an image onto this screen. Also marks all ancestors as modified.
    // Should this be a view or an image, you think?
    void draw_onto_self(NewGUIView* view, DispPoint pos);

    
    // NOTE: Currently it is okay to attach a view completely out of bounds.
    void attach_subview(NewGUIView* view, DispPoint pos);
    // NOTE: Does not delete the view, only remove it from list!
    void remove_subview(NewGUIView* view);

    void move_subview(NewGUIView* view, DispPoint pos);
    
    // Will be true if a subview has been changed.
    bool need_to_refresh() const { return changed; }
    
    /// === DEBUG ===
    void print_children();
    
    SDL_Surface* image;

protected:
    // Displays on image onto self.
    void display_image_on_self(SDL_Surface* source, int w, int h, DispPoint pos);

    void mark_changed();

private:
    bool changed;
    int w,h;
    
    NewGUIView* background;

    struct GUISubview {
        GUISubview(NewGUIView* view_, DispPoint pos_) : view(view_), pos(pos_) {}
        
        NewGUIView* view;
        DispPoint pos;
        
        // Compare only by view pointer.
        bool operator< (const GUISubview& b) const {
            return view < b.view;
        }
        bool operator== (const GUISubview& b) const {
            return view == b.view;
        }
    };
    
    NewGUIView* parent;
    typedef std::list<GUISubview> Subview_list_t;
    Subview_list_t children;

};



#endif
