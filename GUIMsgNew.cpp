//
//  GUIMsgNew.cpp
//  Coin Checkers 2
//
//  Created by Nathan Daly on 11/19/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "GUIMsgNew.h"
#include "GUITimer.h"
#include "GameDisplay.h"
#include "GUIButton.h"
#include "GUITextViews.h"
#include "GUIApp.h"

#include <iostream>
using std::cout; using std::endl;

const SDL_Color color = {0xbb, 0xbb, 0xbb, 0};


namespace GUI {
    
MsgNew::MsgNew(int w_, int h_, const std::string& msg_, Button_ctrs_t buttons_)
:View(w_,h_), msg_text(new TextView(w_-10, h_-10)), buttons(buttons_)
{    
    attach_subview(msg_text, DispPoint(get_w()/2 - msg_text->get_w()/2, 5));
   
    fill_with_color(color);
    set_msg(msg_);
    msg_text->set_text_size(20);
    
    for (size_t i = 0; i < buttons_.size(); i++) {
        add_button(buttons_[i]);
    }
    
}

void MsgNew::set_msg(const std::string msg_) {
    
    msg_text->set_text(msg_);

}
void MsgNew::add_button(Button* button) {
    
    attach_subview(button, DispPoint(get_w() - 180 - (int)buttons.size()*180, get_h()- 30));
    buttons.push_back(button);
}



MsgNew_Scroll_In::MsgNew_Scroll_In(int w_, int h_, const std::string& msg_, Button_ctrs_t buttons_)
:MsgNew(w_,h_, msg_, buttons_)
{ }


void MsgNew_Scroll_In::pop_up(int timeout) 
{
    // Store a copy of the screen for the scroll away.
    GUIImage temp_screen(get_w(), get_h());
    int SCREEN_WIDTH = App::get()->get_screen_size().x;
    SDL_Rect screen_rect = {SCREEN_WIDTH/2 - get_w()/2, 0,
                            get_w(), get_h()};
    SDL_Rect dest_rect = {0, 0, get_w(), get_h()};
    
	SDL_BlitSurface(SDL_GetVideoSurface(), &screen_rect,
                    temp_screen, &dest_rect);
    
    // Scroll in
    double speed = get_h()/30;
    for (double i = -1 * get_h(); i <= 0; i+= speed) {
        speed = (get_h() - i)/30.;
        displayToScreen(get_image_ptr(), SCREEN_WIDTH/2 - get_w()/2, i, 1);
    }
    
    displayToScreen(get_image_ptr(), SCREEN_WIDTH/2 - get_w()/2, 0, 1);

    // wait
    SDL_Event event;
    
    if (timeout >= 0) {
        
        GUITimer t;
        t.start();
        
        do {
            SDL_PollEvent(&event);
        }
        while (t.get_time() < timeout);
    }
    else {
        do {
            SDL_PollEvent(&event);
        }
        while (event.type != SDL_MOUSEBUTTONDOWN);
    }

    // scroll out
    speed = get_h()/30;
    for (double i = 0; i >= -1 * get_h(); i -= speed) {
        speed = (get_h() - (-1.0 * get_h() - i))/30.;
        displayToScreen(temp_screen, SCREEN_WIDTH/2 - get_w()/2, 0, 0);
        displayToScreen(get_image_ptr(), SCREEN_WIDTH/2 - get_w()/2, i, 0);
        SDL_UpdateRects(SDL_GetVideoSurface(), 1, &screen_rect);
    }
    displayToScreen(temp_screen, SCREEN_WIDTH/2 - get_w()/2, 0, 1);

}



//void MsgNew_Scroll_In::pop_up_modal() 
//{
//    pop_up();
//}

} // namespace GUI

