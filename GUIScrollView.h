//
//  GUIScrollView.h
//  Deep
//
//  Created by Nathan Daly on 1/2/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#ifndef Deep_GUIScrollView_h
#define Deep_GUIScrollView_h

#include "GUIView.h"
#include "GUIImageView.h"

class GUIScrollView : public NewGUIView  {
public:
    
    GUIScrollView(int w_, int h_, NewGUIView *display_view_);
    ~GUIScrollView();
    
    void update();
    
    virtual bool handle_mouse_scroll_start(bool up_down);
    virtual bool handle_mouse_scroll_stop(bool up_down);
    
    // Overrided to add or remove the scrollbars.
    virtual void did_resize(int w_, int h_);

private:
    
    class ScrollBarBg : public NewGUIView {
    public:
        ScrollBarBg(int w_, int h_, GUIScrollView *view_) 
        : NewGUIView(w_,h_), view(view_) { }
        
        void display();
        
        virtual bool handle_mouse_down(DispPoint coord);

    private:
        GUIScrollView *view;
    };
    class ScrollBar : public NewGUIView {
    public:
        ScrollBar(int w_, int h_, GUIScrollView *view_) 
        : NewGUIView(w_,h_), view(view_), clicked(false) { }
        
        void display();

        virtual bool handle_mouse_down(DispPoint coord);
        virtual bool handle_mouse_up(DispPoint coord);
        virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion);

    private:
        bool clicked;
        DispPoint click;
        GUIScrollView *view;
    };
    class ScrollArrow : public NewGUIImageView {
    public:
        ScrollArrow(bool up_down_, const GUIImage& image)
        : NewGUIImageView(image), up_down(up_down_) { }
 
        virtual bool handle_mouse_down(DispPoint coord);
        virtual bool handle_mouse_up(DispPoint coord);

    private:
        bool up_down;
    };
    
    
    void move_display_to(DispPoint pos);
    void move_scroll_bar_to(DispPoint pos);
    
    ScrollBarBg scroll_bar_bg;
    ScrollBar scroll_bar;
    ScrollArrow arrow_up, arrow_down;
    NewGUIView *display_view;

    int w_init, h_init;
    
    double scroll_y;
    double scroll_y_vel;

    bool scrollable; // false if this view is larger than display_view
    bool scrolling; // true if currently holding down scroll wheel/gesture
        
    int scroll_bar_x;
    // Where the scroll bar should stop (set after sizes are determined)
    int scroll_bar_top;
    int scroll_bar_bottom;
};



#endif
