//
//  ScrollView.cpp
//  Deep
//
//  Created by Nathan Daly on 1/2/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include "GUIScrollView.h"
#include "GUIApp.h"

#include "Compatibility.h"

#include <iostream>
#include TR1_FUNCTIONAL_H

using namespace std;
using namespace std::tr1;

const int SCROLL_BAR_W = 15;
const int scroll_amount_c = 5;

namespace GUI {
    
ScrollView::ScrollView(int w_, int h_, View *display_view_)
:View(w_,h_), 
scroll_bar(SCROLL_BAR_W, h_ * h_/display_view_->get_h(), this),
scroll_bar_bg(SCROLL_BAR_W, h_, this), 
arrow_up(true, GUIImage("GUIImages/scroll_bar_vert2.bmp")),
arrow_down(false, GUIImage("GUIImages/scroll_bar_vert3.bmp")),
display_view(display_view_),
w_init(w_), h_init(h_),
scroll_y(0), scroll_y_vel(0), scrolling(false)
{
    SDL_Color clear = {0xff, 0, 0xff};
    GUIImage bg = GUIImage::create_filled(w_, h_, clear);
    draw_onto_self(bg, DispPoint());
    set_clear_color(clear);
    
    attach_subview(display_view_, DispPoint());
    
    if (display_view->get_h() <= get_h()) {
        scrollable = false;
        
        resize(min(display_view->get_w(), w_),
               min(display_view->get_h(), h_));
        
        return; // todo Remove to so you can resize later.
    }
    scrollable = true;    
    
    scroll_bar_bg.display();
    scroll_bar.display();
    
    scroll_bar_x = get_w()-scroll_bar_bg.get_w();
    attach_subview(&scroll_bar_bg, DispPoint(scroll_bar_x, 0));
    attach_subview(&arrow_up, DispPoint(scroll_bar_x, scroll_bar_bottom-7)); // HACK!!
    attach_subview(&arrow_down, DispPoint(scroll_bar_x, scroll_bar_bottom-7+arrow_up.get_h())); // HACK!!
    
    attach_subview(&scroll_bar, DispPoint(scroll_bar_x, scroll_bar_top));
    
}
ScrollView::~ScrollView()
{
    if (is_subview(&scroll_bar_bg)) remove_subview(&scroll_bar_bg);
    if (is_subview(&scroll_bar))    remove_subview(&scroll_bar);
    if (is_subview(&arrow_up))      remove_subview(&arrow_up);
    if (is_subview(&arrow_down))    remove_subview(&arrow_down);
}

void ScrollView::update() {
    
    if ((display_view->get_w() < w_init && display_view->get_w() > get_w()) || 
        (display_view->get_h() < h_init && display_view->get_h() > get_h())) {
        
        resize(min(display_view->get_w(), w_init),
               min(display_view->get_h(), h_init));
        
    }
    
    
    if (scroll_y_vel == 0) {
        return;
    }
    
    if (!scrolling) {
        if (scroll_y_vel != 0) {
            scroll_y_vel -= 0.5 * (scroll_y_vel < 0 ? -1 : 1);
        }
        
    }
    
    //    const double display_scroll_range = display_view->get_h() - get_h();
    //    const double scroll_bar_range = scroll_bar_bottom - scroll_bar_top - scroll_bar.get_h();
    
    scroll_y += scroll_y_vel;
    move_display_to(DispPoint(0,scroll_y));
}

void ScrollView::did_resize(int w_, int h_) {
    
    if (display_view->get_h() <= get_h()) {
        scrollable = false;
    }
    else {
        scrollable = true;
    }
    
    //    scroll_bar_bg.display();
    //    scroll_bar.display();
    //    
    //    scroll_bar_x = get_w()-scroll_bar_bg.get_w();
    //    attach_subview(&scroll_bar_bg, DispPoint(scroll_bar_x, 0));
    //    attach_subview(&arrow_up, DispPoint(scroll_bar_x, scroll_bar_bottom-7)); // HACK!!
    //    attach_subview(&arrow_down, DispPoint(scroll_bar_x, scroll_bar_bottom-7+arrow_up.get_h())); // HACK!!
    //    
    //    attach_subview(&scroll_bar, DispPoint(scroll_bar_x, scroll_bar_top));
}

bool ScrollView::handle_mouse_scroll_start(bool up_down) {
    
    cout << "mouse scroll! " << (up_down ? " up" : " down") << endl;
    
    scroll_y_vel = scroll_amount_c * (up_down ? -1 : 1);
    
    scrolling = true;
    
    cout << "scroll:  " << scroll_y << endl;
    
    repeater = App::get()->repeat_on_timer(bind(&ScrollView::update, this), -1);
    
    return true;
}
bool ScrollView::handle_mouse_scroll_stop(bool up_down) {
    
    cout << "mouse scroll release! " << (up_down ? " up" : " down") << endl;
    
    scrolling = false;
    
    App::get()->cancel_timer_op(repeater);
    repeater = 0;
    
    return true;
}

bool ScrollView::ScrollBarBg::handle_mouse_down(DispPoint coord) {
    
    const double scroll_bar_range = view->scroll_bar_bottom - view->scroll_bar_top;
    
    int new_y = (coord.y / scroll_bar_range) * (view->display_view->get_h() - (double)get_h());
    view->move_display_to(DispPoint(0,new_y));
    view->scroll_y_vel = 0;
    
    
    return true;
}

void ScrollView::ScrollBarBg::display() {
    
    static GUIImage top("GUIImages/scroll_bar_vert0.bmp");
    static GUIImage mid("GUIImages/scroll_bar_vert4.bmp");
    static GUIImage bottom("GUIImages/scroll_bar_vert1.bmp");
    
    draw_onto_self(top, DispPoint());
    
    view->scroll_bar_top = top.geth() - 13; // HACK!!
    view->scroll_bar_bottom = get_h() + 7 - (view->arrow_up.get_h()+view->arrow_down.get_h()); // HACK!!
    
    int bottom_of_drawing = view->scroll_bar_bottom-bottom.geth();
    
    for (int i = top.geth(); i < bottom_of_drawing; i+=mid.geth()) {
        draw_onto_self(mid, DispPoint(0,i));
    }    
    draw_onto_self(bottom, DispPoint(0, bottom_of_drawing));
    
}
void ScrollView::ScrollBar::display() {
    
    int h = view->scroll_bar_bottom - view->scroll_bar_top;
    resize(get_w(), h * h/view->display_view->get_h());
    
    static GUIImage top("GUIImages/scroll_bar_vert5.bmp");
    static GUIImage mid("GUIImages/scroll_bar_vert7.bmp");
    static GUIImage bottom("GUIImages/scroll_bar_vert6.bmp");
    
    draw_onto_self(top, DispPoint());
    
    for (int i = top.geth(); i < get_h()-bottom.geth(); i+=mid.geth()) {
        draw_onto_self(mid, DispPoint(0,i));
    }    
    draw_onto_self(bottom, DispPoint(0, get_h()-bottom.geth()));
    
}

bool ScrollView::ScrollBar::handle_mouse_down(DispPoint coord) {
    
    clicked = true;
    click = coord;
    view->scrolling = true;
    capture_focus();
    
    return true;
}
bool ScrollView::ScrollBar::handle_mouse_up(DispPoint coord) {
    
    clicked = false;
    click = coord;
    view->scrolling = false;
    lose_focus();
    
    return true;
}
bool ScrollView::ScrollBar::handle_mouse_motion(DispPoint coord, DispPoint rel_motion) {
    
    if (!clicked) return false;
    
    //    const double scroll_bar_range = view->scroll_bar_bottom - view->scroll_bar_top;
    //    
    //    double offset = coord.y / scroll_bar_range;
    //    double display_range = view->display_view->get_h() - (double)view->get_h();
    //    
    //    int new_y = offset * display_range;
    //    view->scroll_y = new_y;
    //    view->scroll_y_vel = 0;
    
    
    //    const double display_scroll_range = view->display_view->get_h() - view->get_h();
    //    const double scroll_bar_range = view->scroll_bar_bottom - view->scroll_bar_top;
    //    
    //    const double ratio = display_scroll_range / scroll_bar_range;
    //    
    //    int old_pos = get_rel_pos().y;
    //    
    //    int new_pos = old_pos + rel_motion.y;
    //    
    //    
    //    view->scroll_y += rel_motion.y ;
    //    view->scroll_y_vel = 0;
    
    int old_pos = get_rel_pos().y;
    int new_pos = old_pos + rel_motion.y;
    
    cout << "MOTION: " << rel_motion.y << " " << new_pos << endl;
    
    
    DispPoint new_coord = adjust_to_parent(coord - click);
    
    view->move_scroll_bar_to(DispPoint(view->scroll_bar_x, new_coord.y));
    
    
    return true;
}

bool ScrollView::ScrollArrow::handle_mouse_down(DispPoint coord) {
    
    ScrollView* view = (ScrollView*)get_parent();
    int new_y = view->scroll_y;
    new_y += scroll_amount_c * (up_down ? -1 : 1);
    
    view->move_display_to(DispPoint(0, new_y));
    return true;
}
bool ScrollView::ScrollArrow::handle_mouse_up(DispPoint coord) {
    
    // stop the effect of holding the button.    
    return true;
}



void ScrollView::move_display_to(DispPoint pos) {
    
    const double display_scroll_range = display_view->get_h() - get_h();
    const double scroll_bar_range = scroll_bar_bottom - scroll_bar_top - scroll_bar.get_h();
    
    scroll_y = pos.y;
    
    if (scroll_y < 0) {
        scroll_y = 0;
        scroll_y_vel = 0;
    }
    if (scroll_y > display_scroll_range) {
        scroll_y = display_scroll_range;
        scroll_y_vel = 0;
    }
    move_subview(display_view, DispPoint(pos.x, -scroll_y));
    
    int scroll_bar_pos = scroll_bar_top + (scroll_y / display_scroll_range * scroll_bar_range);
    
    move_subview(&scroll_bar, DispPoint(scroll_bar_x, scroll_bar_pos));
    
}
void ScrollView::move_scroll_bar_to(DispPoint pos) {
    
    
    const double display_scroll_range = display_view->get_h() - get_h();
    const double scroll_bar_range = scroll_bar_bottom + 7 - scroll_bar_top - scroll_bar.get_h();
    
    if (pos.y < scroll_bar_top) {
        pos.y = scroll_bar_top;
    }
    if (pos.y > scroll_bar_range) {
        pos.y = scroll_bar_range;
    }
    move_subview(&scroll_bar, pos);
    
    const double ratio = display_scroll_range / scroll_bar_range;
    
    scroll_y = pos.y * ratio;
    
    move_subview(display_view, DispPoint(0, -scroll_y));
    
}

} // namespace GUI
