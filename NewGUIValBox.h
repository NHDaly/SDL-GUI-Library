//
//  GUIValBox.h
//  SDL_Word_Processor
//
//  Created by Nathan Daly on 7/14/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#include "NewGUIView.h"

#include "NewGUITextBox.h"

#include <cctype>   // For isalpha().
#include <sstream>
#include <string>



#ifndef NEW_GUI_VALUE_BOX_H
#define NEW_GUI_VALUE_BOX_H

class NewGUIValue_Box: public NewGUIView {
public:
    
    NewGUIValue_Box(int w_, int h_)
    : NewGUIView(w_,h_)
    {}

    virtual double get_value() const = 0;
};


class NewGUIInteger_Text_Box : public NewGUITextBox {
public:
    
    NewGUIInteger_Text_Box(int w_, int h_)
    : NewGUITextBox(w_,h_) { }
    
	virtual void add_letter(char ltr, int index) {
        if (isdigit(ltr)) NewGUITextBox::add_letter(ltr, index);
    }
};


class NewGUIValue_Text_Box : public NewGUIValue_Box {
public:
    
    NewGUIValue_Text_Box(int w_, int h_, std::string bg_image = "images/bg.bmp");    
    
    virtual double get_value() const;

private:
    
    NewGUIInteger_Text_Box text_box;
};

class NewGUIValue_Slider : public NewGUIValue_Box {
public:
  
    NewGUIValue_Slider(int w_, int h_, int max_ = 1, int min_ = 0)
    : NewGUIValue_Box(w_,h_), value(0), clicked(false),
    min(min_), max(max_)
    { }
    
    virtual ~NewGUIValue_Slider() = 0;

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
    void set_value(double value_) { value = value_; display(); }
    
private:
    double value;
    bool clicked;
    
    double min, max;
};

class NewGUIValue_Horiz_Slider : public NewGUIValue_Slider {
public:
    NewGUIValue_Horiz_Slider(int w_)
    : NewGUIValue_Slider(w_, 20),
    left_edge(0), right_edge(0)
    { }
        
    virtual void display();
    
	virtual bool handle_mouse_down(DispPoint coord);
	virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion);
    
private:
    int left_edge, right_edge;    
};

class NewGUIValue_Vert_Slider : public NewGUIValue_Slider {
public:
    NewGUIValue_Vert_Slider(int h_)
    : NewGUIValue_Slider(20, h_),
    bottom_edge(0), top_edge(0)
    { }
    
    virtual void display();
    
	virtual bool handle_mouse_down(DispPoint coord);
	virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion);
    
private:
    int bottom_edge, top_edge;    
};


// Can either be hooked up to a value_box or can have values entered manually.
class NewGUIValue_Display : public NewGUIView {
public:
    NewGUIValue_Display(int w_, int h_, const NewGUIValue_Box* linked_box = 0);
    
    virtual void display();
    
    void link_value_box(const NewGUIValue_Box* value_box_)
	    { value_box = value_box_; }
    
    const std::string& get_text() { return text; }
    void set_value(double value_) { set_text(value_); }
    
private:
    const NewGUIValue_Box* value_box;
    
    std::string text;
    
    void set_text(double value) {
        std::stringstream value_to_text;
        value_to_text << value;
        text = value_to_text.str();
    }
};


#endif /* NEW_GUI_VALUE_BOX_H */

