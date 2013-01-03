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

#include "GUIUtility.h"
#include "SDL/SDL_video.h"
#include "SDL/SDL_keyboard.h"

struct Unhandled_Click {
    
    Unhandled_Click(DispPoint coord_) : coord(coord_) { }
    DispPoint coord;
};
struct Unhandled_Key {
    
    Unhandled_Key(SDL_keysym key_) : key(key_) { }
    SDL_keysym key;
};

class NewGUIView;
class SDL_Surface;

// This class maintains information about the Window. 
// (i.e name, size, etc.) Has one View instance that shows what is displayed in
// the window.
class NewGUIWindow {
public:

    NewGUIWindow(int w, int h, const std::string& name_ = "",
                 int numColors = 256, const Uint32 flags = SDL_SWSURFACE);
    
    void crop(int w, int h);
    void stretch(int w, int h);
    
    void rename(const std::string& name_);
    
    NewGUIView* get_main_view() { return main_view; }

    DispPoint get_dim();
        
    // Handle subviews to main_view:
    void attach_subview(NewGUIView* view, DispPoint pos);
    void move_subview(NewGUIView* view, DispPoint pos);
    void remove_subview(NewGUIView* view);
    void remove_last_subview(); // Remove subview last added

    
    // If main_view has changed at all, re-display it.
    void refresh();
    
private:
    SDL_Surface* window;
    NewGUIView* main_view;
    std::string name;
    
    int num_colors;
    Uint32 flags;
    
    static bool WINDOW_ALREADY_CREATED;
};

#endif /* NEWGUIWINDOW_H */
