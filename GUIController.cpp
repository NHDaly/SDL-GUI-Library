//
//  NewGUIController.cpp
//  Deep
//
//  Created by Nathan Daly on 1/2/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include <iostream>

#include "GUIController.h"

#include "GUIApp.h"

void NewGUIController::capture_focus() {
    
    if (NewGUIApp::get()->has_focus(this)) return;
    
    NewGUIApp::get()->give_focus(this);
    
    got_focus();
}
void NewGUIController::lose_focus() {
    if (!NewGUIApp::get()->has_focus(this)) return; //throw GUIError("Can't lose_focus if didn't already have it.");
    
    NewGUIApp::get()->release_focus(this);
    
    lost_focus();
}
