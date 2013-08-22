//
//  GUIDropDown.h
//  Deep
//
//  Created by Nathan Daly on 8/22/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#ifndef __Deep__GUIDropDown__
#define __Deep__GUIDropDown__

#include <iostream>
#include <string>
#include <list>

#include "GUIView.h"

namespace GUI {

class DropDownMenu : public View {
public:
    DropDownMenu(int w, int text_size = 12);
    
    // Init with entries (list of std::strings)
    template <typename Iter_t>
    DropDownMenu(int w, int text_size,
                 Iter_t entries_begin, Iter_t entries_end);
    
    std::string get_selected_entry();
    int get_selected_entry_position();
    
private:
    void init_menu_view(int text_size_);
    void update_menu_view();

    int text_size;
    int selected_entry_idx;
    
    std::list<std::string> entries;
    
    View *menu;
    
    bool clicked;
    DispPoint pre_click_pos;
    
    // Mouse Events. Following three functions all work the same:
    //  Returns true if the mouse-event is finished being handled.
    //  If returns false, handling will continue up the chain.
    //  May optionally call capture_focus() to become the target for keypresses.
    virtual bool handle_mouse_down(DispPoint coord);
    virtual bool handle_mouse_up(DispPoint coord);
    virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion);

    friend class MenuEntry;
    
};
    
    template <typename Iter_t>
    DropDownMenu::DropDownMenu(int w, int text_size_,
                               Iter_t entries_begin, Iter_t entries_end)
    :View(w, 1), entries(entries_begin, entries_end)
    {
        init_menu_view(text_size_);
    }

} // namespace GUI

#endif /* defined(__Deep__GUIDropDown__) */
