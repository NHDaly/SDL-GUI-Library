//
//  GUIMsg.h
//  Coin Checkers 2
//
//  Created by Nathan Daly on 11/19/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Coin_Checkers_2_GUIMsg_h
#define Coin_Checkers_2_GUIMsg_h

#include "GUIView.h"

#include <string>
#include <vector>

class GUIMsg : GUIView {
public:
    
    // Size of button_texts must == num_buttons
    GUIMsg(int w_, int h_, const std::string& msg = "", int num_buttons = 0, 
           std::vector<std::string> button_texts = std::vector<std::string>()); 
    
    void set_msg(const std::string msg_);
    
    void pop_up(int timeout = -1); // ms until window disappears. (-1 waits til click)
        
};


#endif
