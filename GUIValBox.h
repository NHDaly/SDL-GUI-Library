//
//  GUIValBox.h
//  SDL_Word_Processor
//
//  Created by Nathan Daly on 7/14/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#include "GUIView.h"
#include "GUIView_Ctrl.h"

#include "GUIText_box.h"

#include <cctype>   // For isalpha().
#include <sstream>
#include <string>



#ifndef GUI_VALUE_BOX_H
#define GUI_VALUE_BOX_H

class GUIValue_Box: public GUIView, public GUIView_Ctrl {
public:
    
    GUIValue_Box(int w_, int h_)
    : GUIView(w_,h_), GUIView_Ctrl(GUIView_shptr_t()) // This is a fake value
    {}

    virtual double get_value() const = 0;
};


class GUIInteger_Text_Box : public GUIText_box {
public:
    
    GUIInteger_Text_Box(int w_, int h_)
    : GUIText_box(w_,h_) { }
    
	virtual void add_letter(char ltr, int index) {
        if (isalpha(ltr)) GUIText_box::add_letter(ltr, index);
    }
};


class GUIValue_Text_Box : public GUIValue_Box {
public:
    
    GUIValue_Text_Box(int w_, int h_, std::string bg_image = "images/bg.bmp");    
    
    virtual void display();
    
    virtual double get_value() const;

private:
    
    GUIInteger_Text_Box text_box;
};

class GUIValue_Slider : public GUIValue_Box {
public:
  
    GUIValue_Slider(int w_, int h_)
    : GUIValue_Box(w_,h_), value(0), clicked(false),
    min(0), max(1)
    { }
    
    virtual ~GUIValue_Slider() = 0;

    void set_range(double min, double max) { min = min; max = max; }
    void set_min(double min) { min = min; }
    void set_max(double max) { max = max; }
    
    double get_min() const { return min; }
    double get_max() const { return max; }
        
    // Return the value between min and max.
    virtual double get_value() const;
    
    // Return a value between 0 and 1.
    virtual double get_percent() const;

	virtual void mouse_click(const SDL_Event& event) { clicked = true; }
    void mouse_click_up(const SDL_Event& event) { clicked = false; }

protected:
    bool get_clicked() const { return clicked; }
    void set_value(double value_) { value = value_; }
    
private:
    double value;
    bool clicked;
    
    double min, max;
};

class GUIValue_Horiz_Slider : public GUIValue_Slider {
public:
    GUIValue_Horiz_Slider(int w_)
    : GUIValue_Slider(w_, 20),
    left_edge(0), right_edge(0)
    { }
        
    virtual void display();
    
	virtual void mouse_click(const SDL_Event& event);
	virtual void mouse_motion(const SDL_Event& event);
    
private:
    int left_edge, right_edge;    
};

class GUIValue_Vert_Slider : public GUIValue_Slider {
public:
    GUIValue_Vert_Slider(int h_)
    : GUIValue_Slider(20, h_),
    bottom_edge(0), top_edge(0)
    { }
    
    virtual void display();
    
	virtual void mouse_click(const SDL_Event& event);
	virtual void mouse_motion(const SDL_Event& event);
    
private:
    int bottom_edge, top_edge;    
};


// Can either be hooked up to a value_box or can have values entered manually.
class GUIValue_Display : public GUIView {
public:
    GUIValue_Display(int w_, int h_, const GUIValue_Box* linked_box = 0)
    : GUIView(w_,h_), value_box(linked_box)
    { }
    
    virtual void display();
    
    void link_value_box(const GUIValue_Box* value_box_)
	    { value_box = value_box_; }
    
    const std::string& get_text() { return text; }
    void set_value(double value_) { set_text(value_); }
    
private:
    const GUIValue_Box* value_box;
    
    std::string text;
    
    void set_text(double value) {
        std::stringstream value_to_text;
        value_to_text << value;
        text = value_to_text.str();
    }
};


#endif /* GUI_VALUE_BOX_H */

