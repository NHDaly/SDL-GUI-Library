//
//  GUIValBox.h
//  SDL_Word_Processor
//
//  Created by Nathan Daly on 7/14/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#ifndef GUI_VALUE_BOX_H
#define GUI_VALUE_BOX_H

#include "GUIView.h"

#include "GUITextViews.h"

#include <cctype>   // For isalpha().
#include <sstream>
#include <string>

namespace GUI { 
    
    
class Value_Box: public GUI::View {
public:
    
    Value_Box(int w_, int h_)
    : GUI::View(w_,h_)
    {}

    virtual double get_value() const = 0;
};


class Integer_Text_Box : public TextField {
public:
    
    Integer_Text_Box(int w_, int h_)
    : TextField(w_,h_) { }
    
	virtual void add_letter(char ltr, int index) {
        if (isdigit(ltr)) TextField::add_letter(ltr, index);
    }
};


class Value_Text_Box : public Value_Box {
public:
    
    Value_Text_Box(int w_, int h_, std::string bg_image = "GUIImages/bg.bmp");    
    
    virtual double get_value() const;

private:
    
    Integer_Text_Box text_box;
};

class Value_Slider : public Value_Box {
public:
  
    Value_Slider(int w_, int h_, double max_ = 1, double min_ = 0, int initial = -1)
    : Value_Box(w_,h_), value(0), clicked(false),
    min(min_), max(max_)
    {
        set_clear_color(default_color_key_c);
        fill_with_color(default_color_key_c);
        if (initial == -1) set_value((max-min)/2);
        else set_value(initial);
    }
    
    virtual ~Value_Slider() = 0;

    virtual void display() {}

    void set_range(double min_, double max_) { min = min_; max = max_; }
    void set_min(double min_) { min = min_; }
    void set_max(double max_) { max = max_; }
    
    double get_min() const { return min; }
    double get_max() const { return max; }
        
    // Return the value between min and max.
    virtual double get_value() const;
    
    void set_new_value(double value_) {
        
        if (value_ > max || value_ < min) throw Error("value out of range");
        set_value((value_-min)/(max-min));
    }

    // Return a value between 0 and 1.
    virtual double get_percent() const;

	virtual bool handle_mouse_down(DispPoint coord) { clicked = true; capture_focus(); return true; }
    virtual bool handle_mouse_up(DispPoint coord) { clicked = false; lose_focus(); return true; }

protected:
    bool get_clicked() const { return clicked; }
    void set_value(double value_) { value = value_; display(); } // (in percent)
    
private:
    double value; // stored as percent between 0 and 1.
    bool clicked;
    
    double min, max;
};

class Value_Horiz_Slider : public Value_Slider {
public:
    Value_Horiz_Slider(int w_)
    : Value_Slider(w_, 20),
    left_edge(0), right_edge(0)
    { display(); }
        
    virtual void display();
    
	virtual bool handle_mouse_down(DispPoint coord);
	virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion);
    
private:
    int left_edge, right_edge;    
};

class Value_Vert_Slider : public Value_Slider {
public:
    Value_Vert_Slider(int h_)
    : Value_Slider(20, h_),
    bottom_edge(0), top_edge(0)
    { display(); }
    
    virtual void display();
    
	virtual bool handle_mouse_down(DispPoint coord);
	virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion);
    
private:
    int bottom_edge, top_edge;    
};
class Value_Joystick_Slider : public Value_Slider {
public:
    Value_Joystick_Slider(int radius_ = 10)
    : Value_Slider(15+2*radius_,15+2*radius_, radius_, 0, 0), edge(0)
    { display(); }
    
    // Get angle in degrees.
    double get_angle() {
        return angle * 360;
    }
    
    virtual void display();
    
	virtual bool handle_mouse_down(DispPoint coord);
	virtual bool handle_mouse_up(DispPoint coord);
	virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion);
    
private:
    double angle;
    double edge;    
};


// Can either be hooked up to a value_box or can have values entered manually.
class Value_Display : public GUI::View {
public:
    Value_Display(int w_, int h_, const Value_Box* linked_box = 0);
    
    virtual void display();
    
    void link_value_box(const Value_Box* value_box_)
	    { value_box = value_box_; }
    
    const std::string& get_text() { return text; }
    void set_value(double value_) { set_text(value_); }
    
private:
    const Value_Box* value_box;
    
    std::string text;
    
    void set_text(double value) {
        std::stringstream value_to_text;
        value_to_text << value;
        text = value_to_text.str();
    }
};

} // namespace GUI

#endif /* NEW_GUI_VALUE_BOX_H */

