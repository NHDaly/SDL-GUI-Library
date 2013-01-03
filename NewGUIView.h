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
#include "NewGUIController.h"

class SDL_Surface;
class NewGUIWindow;
class GUIImage;
/// @todo Hack
#include "GUIImage.h"
#include <iostream>

class NewGUIView : public NewGUIController {
public:
    
    NewGUIView(int w_, int h_);
    virtual ~NewGUIView();
        
    // Redraw any children that have changed onto self. Return true if changed.
    void refresh();
    
    
    // Renders an image onto this screen. Also marks all ancestors as modified.
    // Should this be a view or an image, you think?
    void draw_onto_self(const GUIImage &image, DispPoint pos);

    
    // NOTE: once attached, a subview "belongs" to this view. If this view is
    //   deleted, all subviews are deleted as well.
    // NOTE: Currently it is okay to attach a view completely out of bounds.
    void attach_subview(NewGUIView* view, DispPoint pos);
    // NOTE: Does not delete the view, only remove it from list!
    void remove_subview(NewGUIView* view);
    void remove_last_subview(); // Remove subview last added

    bool is_subview(NewGUIView* view);

    void move_subview(NewGUIView* view, DispPoint pos);
        
    // Will be true if a subview has been changed.
    bool need_to_refresh() const { return changed; }
    
    // Mouse Events: Template Methods
    // Either handle event or pass up to parent.
    // Override handle_mouse_down() to change behavior.
    void mouse_down(DispPoint coord);
    // Override handle_mouse_up() to change behavior.
    void mouse_up(DispPoint coord);
    // Override handle_mouse_motion() to change behavior.
    void mouse_motion(DispPoint coord, DispPoint rel_motion);
    // Override handle_mouse_scroll_start() to change behavior.
    void mouse_scroll_start(bool up_down); // up == true, down == false
    // Override handle_mouse_scroll_stop() to change behavior.
    void mouse_scroll_stop(bool up_down); // up == true, down == false

    // Keyboard Events: Template Methods
    // These don't pass up to parent.
    // Override handle_key_down() to change behavior.
    void key_down(SDL_keysym key);
    // Override handle_key_up() to change behavior.
    void key_up(SDL_keysym key);


    // Returns the deepest subview (could be this) on which coord lies.
    NewGUIView* get_view_from_point(DispPoint coord);
    

    DispPoint get_abs_pos(); // Pos on screen
    DispPoint get_rel_pos(); // Pos on parent
    
    
    int get_w() { return w; }
    int get_h() { return h; }
    
    
    void resize(int w, int h);
        
    
//    // *** The following two functions will call got_focus() and lost_focus(). 
//    // *** Derived behavior may be specified by overriding those two functions.
//    // This function may be optionally called to tell Window to send keyboard
//    // input to this view.
//    void capture_focus();
//    // If focus was captured, this function may be called to release focus.
//    void lose_focus();
    
    
    friend class NewGUIWindow;
    friend class NewGUIApp;
    
protected:
    
    void set_clear_color(SDL_Color clear_color);
    void clear_alpha();
    
    void mark_changed();
    
    // Called if resize() was called on this view.
    virtual void did_resize(int w_, int h_) { }
    
    // Mouse Events. Following three functions all work the same:
    //  Returns true if the mouse-event is finished being handled.
    //  If returns false, handling will continue up the chain.
    //  May optionally call capture_focus() to become the target for keypresses.
    virtual bool handle_mouse_down(DispPoint coord) { return false; }
    virtual bool handle_mouse_up(DispPoint coord) { return false; }
    virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion) { return false; }

    virtual bool handle_mouse_scroll(bool up_down) { return false; }


    // Key Events. Following two functions all work the same:
    //  Returns true if the key-event is finished being handled.
    //  If returns false, handling will continue up the chain.
    virtual bool handle_key_down(SDL_keysym key) { return false; }
    virtual bool handle_key_up(SDL_keysym key) { return false; }


//    // These functions will be called by capture/lose focus, and may be
//    // overridden to provide behavior on focus gain/loss.
//    virtual void got_focus() { }
//    virtual void lost_focus() { }

    // Hierarchy
    NewGUIView* get_parent() { return parent; }
//    void move_to_rel_pos(DispPoint pos_) { pos = pos_; parent->mark_changed(); }

    DispPoint abs_from_rel(DispPoint coord);
    DispPoint adjust_to_parent(DispPoint coord);    
    DispPoint adjust_to_rel(DispPoint coord);    

    // returns true if coord is within this view's rectangle.
    bool rel_point_is_on_me(DispPoint coord);
    bool abs_point_is_on_me(DispPoint coord);

    const SDL_Surface* get_image_ptr() { return image; }
    
private:
    bool changed;
    int w,h;
    DispPoint pos;
            
    SDL_Surface* image;
    SDL_Surface* display;   // includes children drawn on.
    
    bool is_alpha;     
    SDL_Color clear_color; // only valid if is_alpha == true
    
    // Draws image onto display.
    void render_image(SDL_Surface* source, int w, int h, DispPoint pos);
    

    // Hierarchy
    NewGUIView* parent;
    typedef std::list<NewGUIView*> Subview_list_t;
    Subview_list_t children;
                
    
    // returns the deepest view that lies under coord, and its depth.
    // (0 is THIS, 1 is a child, 2 is grandchild, etc.)
    typedef std::pair<NewGUIView*, int> View_Depth_t;
    View_Depth_t deepest_view_from_point(DispPoint coord, int depth);
    
    friend bool x_then_y_view_less_than(const NewGUIView* a, const NewGUIView* b);
};


#endif
