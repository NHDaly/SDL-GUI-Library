//
//  GUIController.cpp
//  Deep
//
//  Created by Nathan Daly on 1/2/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include <iostream>

#include "GUIController.h"

#include "GUIApp.h"

using GUI::App;

void GUI::Controller::capture_focus() {
    
    if (App::get()->has_focus(this)) return;
    
    App::get()->give_focus(this);
    
    got_focus();
}
void GUI::Controller::lose_focus() {
    if (!App::get()->has_focus(this)) return; //throw Error("Can't lose_focus if didn't already have it.");
    
    App::get()->release_focus(this);
    
    lost_focus();
}
