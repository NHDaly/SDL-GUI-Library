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



#ifndef GUI_VALUE_BOX_H
#define GUI_VALUE_BOX_H

class GUIValue_Box: public GUIView, public GUIView_Ctrl {
public:
    
    GUIValue_Box(int w_, int h_)
    : GUIView(w_,h_), GUIView_Ctrl(GUIView_shptr_t()) // This is a fake value
    {}

    virtual int getValue() = 0;
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
    
    virtual int getValue();

private:
    
    GUIInteger_Text_Box text_box;
};

class GUIValue_Slider : public GUIValue_Box {
public:
  
    GUIValue_Slider(int w_, int h_)
    : GUIValue_Box(w_,h_) { }
    
    virtual void display();
    
    virtual int getValue();

};


#endif /* GUI_VALUE_BOX_H */

