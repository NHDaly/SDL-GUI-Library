//
//  GUIValBox.cpp
//  SDL_Word_Processor
//
//  Created by Nathan Daly on 7/14/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#include "GUIValBox.h"
#include "GameDisplay.h"

using std::string;


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
    
	display_image(text_box.screen, screen, DispPoint(5,5), 0);	
}

void GUIValue_Text_Box::get() {
    
	display_image(text_box.screen, screen, DispPoint(5,5), 0);	
}
