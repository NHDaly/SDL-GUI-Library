//
//  Header.h
//  GUI_Showcase
//
//  Created by Nathan Daly on 2/19/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#ifndef GUI_GUISpreadsheet_h
#define GUI_GUISpreadsheet_h

#include "GUIView.h"

class GUICellsView;
class GUISpreadsheet : public GUIView {
public:
    GUISpreadsheet(int w, int h);
    
private:
    GUICellsView *cells_view;
};



#endif
