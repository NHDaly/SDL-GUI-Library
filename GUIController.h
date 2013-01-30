//
//  NewGUIController.h
//  Deep
//
//  Created by Nathan Daly on 1/2/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#ifndef Deep_NewGUIController_h
#define Deep_NewGUIController_h

#include "Compatibility.h"
#include "GUIUtility.h"

// This class is seperated out from GUIView to allow creation of a class to 
// receive mouse/key events without having to associate it with a displayed view.
class NewGUIController {
	
public:	
	virtual ~NewGUIController() {}

    // Mouse Events. Following three functions all work the same:
    //  Return true if the mouse-event is finished being handled, false otherwise.
    virtual bool handle_mouse_down(DispPoint coord) { return false; }
    virtual bool handle_mouse_up(DispPoint coord) { return false; }
    virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion) { return false; }

    //  up == true, down == false.
    virtual bool handle_mouse_scroll_start(bool up_down) { return false; }
    virtual bool handle_mouse_scroll_stop(bool up_down) { return false; }

    
    
    
    // Key Events. Following two functions all work the same:
    //  Return true if the key-event is finished being handled, false otherwise.
    virtual bool handle_key_down(SDL_keysym key) { return false; }
    virtual bool handle_key_up(SDL_keysym key) { return false; }
	
    
    // These functions will be called by capture/lose focus, and may be
    // overridden to provide behavior on focus gain/loss.
    virtual void got_focus() { }
    virtual void lost_focus() { }

    // *** The following two functions will call got_focus() and lost_focus(). 
    // *** Derived behavior may be specified by overriding those two functions.
    // This function may be optionally called to tell the Application to send 
    //  mouse and keyboard input to this controller.
    void capture_focus();
    // If focus was captured, this function may be called to release focus.
    void lose_focus();
};


#endif
