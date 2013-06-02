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
#include <cmath>

using std::string;
using std::stringstream;
using std::cout; using std::endl;

using GUI::View;

const int MIN_TEXT_BOX_WIDTH = 10;
const int MIN_TEXT_BOX_HEIGHT = 10;

GUIValue_Text_Box::GUIValue_Text_Box(int w_, int h_, string bg_image)
: GUIValue_Box(w_,h_), 
text_box(w_ - 5, h_ - 5)
{
    GUIImage bg = GUIImage::create_blank(500,500);
    SDL_FillRect(bg, 0, SDL_MapRGB(bg->format, 210, 210, 210));
    
    draw_onto_self(bg, DispPoint());
    
    attach_subview(&text_box, DispPoint(5,5));
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
    
    return value * max - min;
}
double GUIValue_Slider::get_percent() const {
    
    return value;
}

const DispPoint HORIZ_SLIDER_DIM (1,5);
const DispPoint HORIZ_SLIDER_CAP_DIM (3,5);

void GUIValue_Horiz_Slider::display() {

    fill_with_color(default_color_key_c);

	DispPoint loc;
	loc.x = 0;
	loc.y = get_h()/2 - HORIZ_SLIDER_DIM.y/2;
	
	//Display left cap:
    static GUIImage left_cap("GUIImages/slider_horiz_L.bmp", true);
	draw_onto_self(left_cap, loc);
	loc.x += HORIZ_SLIDER_CAP_DIM.x;
	
	//Display center_pieces:
	for (int i = 0; i < get_w()-4; i++){
        static GUIImage middle("GUIImages/slider_horiz_M.bmp", true);
        draw_onto_self(middle, loc);
		loc.x += HORIZ_SLIDER_DIM.x;
	}
	
	//Display right cap:
    static GUIImage right_cap("GUIImages/slider_horiz_R.bmp", true);
	draw_onto_self(right_cap, loc);

    //Dipslay Bubble
    static GUIImage bubble("GUIImages/slider_bubble.bmp", true);
    DispPoint position(get_percent() * get_w(), get_h()/2);
    
    draw_onto_self(bubble, DispPoint(position.x - bubble.getw()/2,
                                     position.y - bubble.geth()/2));
}

bool GUIValue_Horiz_Slider::handle_mouse_down(DispPoint coord) {
    
    GUIValue_Slider::handle_mouse_down(coord);
    
    set_value(static_cast<double>(coord.x) / get_w());

    return true;
}

bool GUIValue_Horiz_Slider::handle_mouse_motion(DispPoint coord, DispPoint rel_motion) {
    
    if (!get_clicked()) return false;
    if (coord.x < left_edge && get_percent() == 0) return false;
    if (coord.x > right_edge && get_percent() == 1) return false;    
    
    set_value(double(coord.x) / get_w());
    if (get_percent() < 0) {
        set_value(0);
        left_edge = coord.x;
    }
    if (get_percent() > 1) { 
        set_value(1);
        right_edge = coord.x;
    }
    return true;
}

void GUIValue_Vert_Slider::display() {
    
    fill_with_color(default_color_key_c);
    
    static GUIImage bubble("GUIImages/slider_bubble.bmp", true);
    
    DispPoint position(get_w()/2, get_percent() * get_h());
    
    draw_onto_self(bubble, DispPoint(position.x - bubble.getw()/2,
                                     position.y - bubble.geth()/2));
}


bool GUIValue_Vert_Slider::handle_mouse_down(DispPoint coord) {
    
    GUIValue_Slider::handle_mouse_down(coord);
    
    set_value(static_cast<double>(coord.y) / get_w());
    
    capture_focus();
    
    return true;
}

bool GUIValue_Vert_Slider::handle_mouse_motion(DispPoint coord, DispPoint rel_motion) {
    
    if (!get_clicked()) return false;
    if (coord.y < bottom_edge && get_percent() == 0) return false;
    if (coord.y > top_edge && get_percent() == 1) return false;    
    
    set_value(get_percent() + static_cast<double>(rel_motion.y) / get_h());
    if (get_percent() < 0) {
        set_value(0);
        bottom_edge = coord.y;
    }
    if (get_percent() > 1) { 
        set_value(1);
        top_edge = coord.y;
    }
    
    return true;
}


void GUIValue_Joystick_Slider::display() {
    
    fill_with_color(default_color_key_c);

    DispPoint center(get_w()/2, get_h()/2);

    //Display left cap:
    static GUIImage left_cap("GUIImages/slider_horiz_L.bmp", true);
	draw_onto_self(left_cap, DispPoint(get_w()/2 - left_cap.getw(), 
                                       get_h()/2 - left_cap.geth()/2));
	
	//Display right cap:
    static GUIImage right_cap("GUIImages/slider_horiz_R.bmp", true);
	draw_onto_self(right_cap, DispPoint(get_w()/2, 
                                       get_h()/2 - right_cap.geth()/2));
    

    
    static GUIImage bubble("GUIImages/slider_bubble.bmp", true);
    
    double radius = get_max() * get_percent();
    cout << "radius: "<< radius << endl;
    double radians = angle * 2*M_PI;
    cout << "angle: "<< angle << endl;
    cout << "radians: "<< radians << endl;
    double x = radius * cos(radians);
    double y = -1 * radius * sin(radians);
    
    cout << x << endl;
    cout << y << endl;
    
    DispPoint position = center + DispPoint(x, y);
    
    draw_onto_self(bubble, DispPoint(position.x - bubble.getw()/2,
                                     position.y - bubble.geth()/2));
}


bool GUIValue_Joystick_Slider::handle_mouse_down(DispPoint coord) {
    
    GUIValue_Slider::handle_mouse_down(coord);
    
    DispPoint center(get_w()/2, get_h()/2);
    DispPoint offset = coord - center;
    
    double radians = atan2(-offset.y, offset.x);
    if (radians < 0) { radians += 2*M_PI; }   // get into range (0, 2*Pi]
    
    angle = radians / (2*M_PI);
    
    return true;
}

bool GUIValue_Joystick_Slider::handle_mouse_up(DispPoint coord) {
    
    GUIValue_Slider::handle_mouse_up(coord);
    
    set_value(0);
    
    return true;
}

bool GUIValue_Joystick_Slider::handle_mouse_motion(DispPoint coord, DispPoint rel_motion) {
    
    if (!get_clicked()) return false;
    
    DispPoint center(get_w()/2, get_h()/2);
    DispPoint offset = coord - center;
    
    double radians = atan2(-offset.y, offset.x);
    if (radians < 0) { radians += 2*M_PI; }   // get into range (0, 2*Pi]
        
    angle = radians / (2*M_PI);

    double length = sqrt((double)offset.x*offset.x + (double)offset.y*offset.y);
    if (length >= get_max()) length = get_max();

//    if (length > edge && get_percent() == 1) return false;    

    set_new_value(length);
    cout << "length: "<< length << endl;
   
    if (get_percent() < 0) {
        set_value(0);
    }
    if (get_percent() > 1) { 
        set_value(1);
        edge = length;        
    }
    
    return true;
}





#include "GUIApp.h"
#include <tr1/functional>
using std::tr1::bind;
using GUI::App;

GUIValue_Display::GUIValue_Display(int w_, int h_, const GUIValue_Box* linked_box)
: View(w_,h_), value_box(linked_box)
{
    App::get()->repeat_on_timer(bind(&GUIValue_Display::display, this), -1);
}

void GUIValue_Display::display() {
    
    fill_with_color(default_color_key_c);
    set_clear_color(default_color_key_c);

    if (value_box) {
        set_text(value_box->get_value());
    }
    
    if (text == "") return;
    
    SDL_Surface *text_surf = createText(text);
    if (text_surf == 0) return;
    
    draw_onto_self(GUIImage(text_surf), DispPoint(2, 2));
}



