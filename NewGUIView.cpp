//
//  NewGUIView.cpp
//  Deep
//
//  Created by Nathan Daly on 9/10/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "NewGUIView.h"

#include <iostream>
using namespace std;

void NewGUIView::draw_onto_self(NewGUIView* view, DispPoint pos) {
    
    // Mark change.
    changed = true;
    
    if (parent) parent->changed = true;
    
    // Render the image onto self.
    /// @todo DISPLAY IMAGE
    cout << "redrawing " << this << endl;
}
