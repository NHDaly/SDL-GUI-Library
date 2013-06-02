//
//  GUIView.h
//  Deep
//
//  Created by Nathan Daly on 9/10/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_GUIView_h
#define Deep_GUIView_h

#include <list>

#include "GUIUtility.h"
#include "GUIController.h"

class SDL_Surface;
class GUIImage;
/// @todo Hack
#include "GUIImage.h"
#include <iostream>

namespace GUI {
    class Window;
    class App;

class View : public GUIController {
public:
    
    View(int w_, int h_);
    virtual ~View();
        
    // Redraw any children that have changed onto self. Return true if changed.
    void refresh();
    
    
    // Renders an image onto this screen. Also marks all ancestors as modified.
    // Should this be a view or an image, you think?
    void draw_onto_self(const GUIImage &image, DispPoint pos);

    void fill_with_color(SDL_Color color);
    
    // NOTE: once attached, a subview "belongs" to this view. If this view is
    //   deleted, all subviews are deleted as well.
    // NOTE: Currently it is okay to attach a view completely out of bounds.
    void attach_subview(View* view, DispPoint pos);
    // NOTE: Does not delete the view, only remove it from list!
    void remove_subview(View* view);
    void remove_last_subview(); // Remove subview last added

    bool is_subview(View* view) const;

    void move_subview(View* view, DispPoint pos);
        
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
    View* get_view_from_point(DispPoint coord) const;

    DispPoint get_abs_pos() const; // Pos on screen
    DispPoint get_rel_pos() const; // Pos on parent
    
    // returns true if coord is within this view's rectangle.
    bool rel_point_is_on_me(DispPoint coord) const;
    bool abs_point_is_on_me(DispPoint coord) const;
    

    int get_w() const { return w; }
    int get_h() const { return h; }
    
    
    void resize(int w, int h);
        
    void set_clear_color(SDL_Color clear_color);
    void clear_alpha();

    bool has_alpha_color() const { return is_alpha; }
    // Only valid if has_alpha_color.
    SDL_Color get_clear_color() const { return clear_color; }

    // returns true if pixel at coord is of clear color.
    // If coord is not on view, or alhpa not set, will return false.
    bool point_is_clear(DispPoint coord) const;
    
    
//    // *** The following two functions will call got_focus() and lost_focus(). 
//    // *** Derived behavior may be specified by overriding those two functions.
//    // This function may be optionally called to tell Window to send keyboard
//    // input to this view.
//    void capture_focus();
//    // If focus was captured, this function may be called to release focus.
//    void lose_focus();
    
    
    friend class GUI::Window;
    friend class GUI::App;
    
protected:
    
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
    View* get_parent() { return parent; }
//    void move_to_rel_pos(DispPoint pos_) { pos = pos_; parent->mark_changed(); }

    // Convert a point to abs, or coordinates relative to this view or parent view.
    DispPoint abs_from_rel(DispPoint coord) const;
    DispPoint adjust_to_parent(DispPoint coord) const;    
    DispPoint adjust_to_rel(DispPoint coord) const;    

    const SDL_Surface* get_image_ptr() const { return image; }
    
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
    View* parent;
    typedef std::list<View*> Subview_list_t;
    Subview_list_t children;
                
    
    // returns the deepest view that lies under coord, and its depth.
    // (0 is THIS, 1 is a child, 2 is grandchild, etc.)
    typedef std::pair<View*, int> View_Depth_t;
    View_Depth_t deepest_view_from_point(DispPoint coord, int depth);
    
    friend bool x_then_y_view_less_than(const View* a, const View* b);
};

} // namespace GUI

#endif
