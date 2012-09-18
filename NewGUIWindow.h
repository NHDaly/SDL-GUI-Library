//
//  NewGUIWindow.h
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef NEWGUIWINDOW_H
#define NEWGUIWINDOW_H


#include <string>

class NewGUIView;
class SDL_Surface;

// This class maintains information about the Window. 
// (i.e name, size, etc.) Has one View instance that shows what is displayed in
// the window.
class NewGUIWindow {
public:
    
    NewGUIWindow(NewGUIView *main_view_, const std::string& name_ = "NewGUIWindow",
                 int numColors = 256);
    NewGUIWindow(int w, int h, const std::string& name_ = "NewGUIWindow",
                 int numColors = 256);
    
    void crop(int w, int h);
    void stretch(int w, int h);
    
    void rename(const std::string& name_);
    
    void set_main_view(NewGUIView* main_view_);
    
    // If main_view has changed at all, re-display it.
    void refresh();
    
private:
    NewGUIView* main_view;
    SDL_Surface* window;
    std::string name;
    
    void initialize(int w, int h, const std::string& name_ = "",
                    int numColors = 256, NewGUIView* main_view_ = 0);
};

#endif /* NEWGUIWINDOW_H */
