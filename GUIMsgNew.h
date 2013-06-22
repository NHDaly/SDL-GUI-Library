//
//  GUIMsgNew.h
//  Coin Checkers 2
//
//  Created by Nathan Daly on 11/19/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Coin_Checkers_2_GUIMsgNew_h
#define Coin_Checkers_2_GUIMsgNew_h

#include "GUIView.h"

#include <string>
#include <vector>


namespace GUI {

class Button;
class TextView;

class MsgNew : public View {
public:
    typedef std::vector<Button*> Button_ctrs_t;
    
    // Size of button_texts must == num_buttons
    MsgNew(int w_, int h_, const std::string& msg = "", Button_ctrs_t buttons = Button_ctrs_t()); 
    
    void set_msg(const std::string msg_);

    void add_button(GUI::Button* button);

    virtual void pop_up(int timeout = -1) = 0; // ms until window disappears.

    // Note, this is not yet implemented.
//    virtual void pop_up_modal(); // stop everything else.

private:
    
    std::vector<Button*> buttons;
    TextView *msg_text;
};

class MsgNew_Scroll_In : public MsgNew {
public:
    typedef std::vector<Button*> Button_ctrs_t;

    // Size of button_texts must == num_buttons
    MsgNew_Scroll_In(int w_, int h_, const std::string& msg = "", Button_ctrs_t buttons = Button_ctrs_t()); 
        
    virtual void pop_up(int timeout = -1) = 0; // ms until window disappears.

//    virtual void pop_up_modal(); // stop everything else.

};

} // namespace GUI

#endif
