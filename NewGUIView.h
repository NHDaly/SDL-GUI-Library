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
class NewGUIWindow;

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
    
    // Template Method
    void mouse_click(DispPoint coord);

    // Returns the deepest subview (could be this) on which coord lies.
    NewGUIView* get_view_from_point(DispPoint coord);
    
    DispPoint get_abs_pos();
    DispPoint get_rel_pos();    
    
    friend class NewGUIWindow;
    
protected:
    // Displays on image onto self.
    void display_image_on_self(SDL_Surface* source, int w, int h, DispPoint pos);

    void mark_changed();
    
    // Returns true if the mouse_down is finished being handled.
    // If returns false, handling will continue up the chain.
    virtual bool handle_mouse_down(DispPoint coord) { return false; }

private:
    bool changed;
    int w,h;
    DispPoint pos;
    
    NewGUIView* background;
    
    NewGUIView* parent;
    typedef std::list<NewGUIView*> Subview_list_t;
    Subview_list_t children;

    ///@todo PRIVATE:
    SDL_Surface* image;

    // returns true if coord is within this view's rectangle.
    bool rel_point_is_on_me(DispPoint coord);
    bool abs_point_is_on_me(DispPoint coord);
    
    DispPoint abs_from_rel(DispPoint coord);
    DispPoint adjust_to_rel(DispPoint coord);    

    
    // returns the deepest view that lies under coord, and its depth.
    // (0 is THIS, 1 is a child, 2 is grandchild, etc.)
    typedef std::pair<NewGUIView*, int> View_Depth_t;
    View_Depth_t deepest_view_from_point(DispPoint coord, int depth);
    
    friend bool x_then_y_view_less_than(const NewGUIView* a, const NewGUIView* b);
};



#endif
