//
//  GUIButton.h
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_GUIButton_h
#define Deep_GUIButton_h

#include "GUIImageView.h"

class QuitAction {
public:
    void operator()() {
        throw QuitAction();
    }
};

#include <iostream>

class GUIButton : public GUIImageView { 
public:
    
    GUIButton()
    : GUIImageView(GUIImage("GUIImages/button.bmp")), 
    is_pressed(false), is_hovered(false),
    image(GUIImage("GUIImages/button.bmp")), hovered_image(GUIImage("GUIImages/button2.bmp")), 
    clicked_image(GUIImage("GUIImages/button3.bmp"))
    {
        SDL_Color clear = {0,0xff,0};
        set_clear_color(clear);
    }
    
protected:
    
    // What to do when button is clicked
    virtual void operation() {
        // Default action is to do nothing.
    }
    
    virtual void set_image(const GUIImage& image_) {
        image = image_;
    }
    virtual void set_hovered_image(const GUIImage& image_) {
        hovered_image = image_;
    }
    virtual void set_clicked_image(const GUIImage& image_) {
        clicked_image = image_;
    }
                                                        
    // Returns true if the mouse_down is finished being handled.
    // If returns false, handling will continue up the chain.
    virtual bool handle_mouse_down(DispPoint coord) { 
        is_pressed = true;
        draw_onto_self(clicked_image, DispPoint());
        is_hovered = true;
        
        capture_focus();
        return true;
    }
    virtual bool handle_mouse_up(DispPoint coord) { 
        // Only perform event on mouse release.
        lose_focus();

        if (is_pressed) {
            is_pressed = false;
            draw_onto_self(image, DispPoint());
            if (is_hovered) {
                operation(); // Do what you were born to do!
            }
        }
        
        if (is_hovered) {
            is_hovered = false;
            return true;
        }
        else return false;

    }
    virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion) { 
        // Only perform event on mouse release.

        if (is_pressed && !is_hovered && rel_point_is_on_me(coord)) {
            draw_onto_self(clicked_image, DispPoint());
            is_hovered = true;
        }
        else if (!is_pressed && !is_hovered && rel_point_is_on_me(coord)) {
            draw_onto_self(hovered_image, DispPoint());
            is_hovered = true;
            capture_focus();
        }
        else if (is_hovered && !rel_point_is_on_me(coord)) {
            draw_onto_self(image, DispPoint());
            is_hovered = false;
            if (!is_pressed)
                lose_focus();
            
        }

        return false;
    }
                                   
private:
    bool is_pressed, is_hovered;
    GUIImage image, hovered_image, clicked_image;
};

#include "GUITextViews.h"

class GUITextButton : public GUIButton { 
public:
    GUITextButton(const std::string &button_text_ = "")
    {
        button_text = new GUITextView(get_w()-30, get_h());
        button_text->set_text(button_text_);
        button_text->set_text_size(16);
        attach_subview(button_text, DispPoint(get_w()-button_text->get_w(), 0));
    }
    
    GUITextView* get_text_view() { return button_text; }
    void set_text(const std::string &button_text_) { button_text->set_text(button_text_); }
    
public:
    GUITextView *button_text;
};


class NoAction {
public:
    void operator()() { }
};
template <typename Oper = NoAction>
class GUIActionButton : public GUITextButton {
public:

    GUIActionButton(Oper oper = Oper())
	:oper(oper) { }

    virtual void operation() {
        oper();
    }
    
private:
	Oper oper;	

};

template <typename Oper>
GUITextButton* GUI_create_button(Oper oper) {
    
    return new GUIActionButton<Oper>(oper);
}

#endif
