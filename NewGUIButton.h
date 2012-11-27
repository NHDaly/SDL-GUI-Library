//
//  NewGUIButton.h
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_NewGUIButton_h
#define Deep_NewGUIButton_h

#include "NewGUIImageView.h"

class QuitAction {
public:
    void operator()() {
        throw QuitAction();
    }
};

class NewGUIButton : public NewGUIImageView { 
public:
    NewGUIButton()
    : NewGUIImageView(GUIImage("images/button.bmp")) 
    {}
    
protected:
    // Returns true if the mouse_down is finished being handled.
    // If returns false, handling will continue up the chain.
    virtual bool handle_mouse_down(DispPoint coord) { throw QuitAction(); }
                                   
private:
};

#endif
