//
//  GUIValBox.cpp
//  SDL_Word_Processor
//
//  Created by Nathan Daly on 7/14/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#include "GUIValBox.h"
#include "GameDisplay.h"

#include "SDL/SDL.h"

#include <iostream>

using std::string;
using std::stringstream;
using std::cout; using std::endl;

const int MIN_TEXT_BOX_WIDTH = 10;
const int MIN_TEXT_BOX_HEIGHT = 10;

GUIValue_Text_Box::GUIValue_Text_Box(int w_, int h_, string bg_image)
: GUIValue_Box(w_,h_), 
text_box(w_ - 5, h_ - 5)
{
//    if (text_box.get_w() <= MIN_TEXT_BOX_WIDTH) 
//        text_box.set(MIN_TEXT_BOX_WIDTH);
//    
//    if (text_box.get_h() <= MIN_TEXT_BOX_HEIGHT) 
//        text_box.set_h(MIN_TEXT_BOX_HEIGHT);
    set_bg(bg_image);
}

void GUIValue_Text_Box::display() {
    
	draw_onto_view(text_box.screen, DispPoint(5,5));	
}

double GUIValue_Text_Box::get_value() const {
    
    stringstream text_to_int;
    text_to_int << text_box.get_text();
    
    int result;
    text_to_int >> result;
    
    return result;
}

GUIValue_Slider::~GUIValue_Slider() { }


double GUIValue_Slider::get_value() const {
    
    return value * (max - min);
}
double GUIValue_Slider::get_percent() const {
    
    return value;
}

const DispPoint HORIZ_SLIDER_DIM (1,5);
const DispPoint HORIZ_SLIDER_CAP_DIM (3,5);

void GUIValue_Horiz_Slider::display() {
    
	DispPoint loc;
	loc.x = 0;
	loc.y = get_h()/2 - HORIZ_SLIDER_DIM.y/2;
	
	//Display left cap:
    static GUIImage left_cap("GUIImages/slider_horiz_L.bmp", true);
	draw_onto_view(left_cap, loc);
	loc.x += HORIZ_SLIDER_CAP_DIM.x;
	
	//Display center_pieces:
	for (int i = 0; i < get_w()-4; i++){
        static GUIImage middle("GUIImages/slider_horiz_M.bmp", true);
        draw_onto_view(middle, loc);
		loc.x += HORIZ_SLIDER_DIM.x;
	}
	
	//Display right cap:
    static GUIImage right_cap("GUIImages/slider_horiz_R.bmp", true);
	draw_onto_view(right_cap, loc);

    //Dipslay Bubble
    static GUIImage bubble("GUIImages/slider_bubble.bmp", true);
    DispPoint position(get_percent() * get_w(), get_h()/2);
    
    draw_onto_view(bubble, DispPoint(position.x - bubble.getw()/2,
                                     position.y - bubble.geth()/2));
}

void GUIValue_Horiz_Slider::mouse_click(const SDL_Event& event) {
    
    GUIValue_Slider::mouse_click(event);
    
    set_value(static_cast<double>(event.button.x) / get_w());
}

void GUIValue_Horiz_Slider::mouse_motion(const SDL_Event& event) {
    
    if (!get_clicked()) return;
    if (event.motion.x < left_edge && get_percent() == 0) return;
    if (event.motion.x > right_edge && get_percent() == 1) return;    
    
    set_value(get_percent() + static_cast<double>(event.motion.xrel) / get_w());
    if (get_percent() < 0) {
        set_value(0);
        left_edge = event.motion.x;
    }
    if (get_percent() > 1) { 
        set_value(1);
        right_edge = event.motion.x;
    }
}

void GUIValue_Vert_Slider::display() {
    
    static GUIImage bubble("GUIImages/slider_bubble.bmp", true);
    
    DispPoint position(get_w()/2, get_percent() * get_h());
    
    draw_onto_view(bubble, DispPoint(position.x - bubble.getw()/2,
                                     position.y - bubble.geth()/2));
}


void GUIValue_Vert_Slider::mouse_click(const SDL_Event& event) {
    
    GUIValue_Slider::mouse_click(event);
    
    set_value(static_cast<double>(event.button.y) / get_w());
}

void GUIValue_Vert_Slider::mouse_motion(const SDL_Event& event) {
    
    if (!get_clicked()) return;
    if (event.motion.y < bottom_edge && get_percent() == 0) return;
    if (event.motion.y > top_edge && get_percent() == 1) return;    
    
    set_value(get_percent() + static_cast<double>(event.motion.yrel) / get_h());
    if (get_percent() < 0) {
        set_value(0);
        bottom_edge = event.motion.y;
    }
    if (get_percent() > 1) { 
        set_value(1);
        top_edge = event.motion.y;
    }
}


void GUIValue_Display::display() {
    
    if (value_box) {
        set_text(value_box->get_value());
    }
    
    draw_onto_view(createText(text), DispPoint(2, 2));
}



