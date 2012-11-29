//
//  NewGUIButton.h
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_NewGUIButton_h
#define Deep_NewGUIButton_h

#include "NewGUIImageView.h"

class QuitAction {
public:
    void operator()() {
        throw QuitAction();
    }
};

#include <iostream>

class NewGUIButton : public NewGUIImageView { 
public:
    
    NewGUIButton()
    : NewGUIImageView(GUIImage("images/button.bmp")), 
    is_pressed(false), is_hovered(false),
    image(GUIImage("images/button.bmp")), hovered_image(GUIImage("images/button2.bmp")), 
    clicked_image(GUIImage("images/button3.bmp"))
    {}
    
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
        if (is_pressed) {
            if (is_hovered) {
                operation(); // Do what you were born to do!
            }
            is_pressed = false;
            draw_onto_self(image, DispPoint());
        }
        lose_focus();
        
        if (is_hovered) return true;
        else return false;

    }
    virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion) { 
        // Only perform event on mouse release.

        std::cout << "HANDLING MOUSE MOTION BIZITCH " << std::endl;
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

class NewGUIQuitButton : public NewGUIButton {
public:
    virtual void operation() {
        throw QuitAction();
    }
};

#endif
