//
//  GUIDropDown.cpp
//  Deep
//
//  Created by Nathan Daly on 8/22/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include "GUIDropDownMenu.h"
#include "GUIButton.h"
#include "GUITextViews.h"

using std::list;
using std::string;

namespace GUI {

    
class MenuEntry : public TextButton {
public:
    MenuEntry(const string &text, int text_size, DropDownMenu &menu_, int index_)
    :TextButton(text), menu(menu_), index(index_)
    {
        button_text->set_text_size(text_size);

        set_image(GUIImage("GUIImages/dropdown_row.bmp"));
        set_hovered_image(GUIImage("GUIImages/dropdown_row_hl.bmp"));
        set_clicked_image(GUIImage("GUIImages/dropdown_row_hl.bmp"));
    }
    
    virtual void operation() {
        
        menu.selected_entry_idx = index;
    }
    
    // overridden to return false so Menu can receive click
    virtual bool handle_mouse_down(DispPoint coord) {
        // run parent mouse up to call operation();
        TextButton::handle_mouse_down(coord);
        
        // return false so Menu can be clicked.
        return false;
    }

    // overridden to return false so Menu can receive click
    virtual bool handle_mouse_up(DispPoint coord) {
        // run parent mouse up to call operation();
        TextButton::handle_mouse_up(coord);
        
        // return false so Menu can be clicked.
        return false;
    }

    // overridden to not highlight if menu isn't open.
    virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion) {
        
        if (!menu.clicked) {
            return false;
        }
        return TextButton::handle_mouse_motion(coord, rel_motion);
    }

private:
    DropDownMenu &menu;
    int index;
};
    
    
    
    
    
    
    
const int vertical_padding = 5; // extra space above/below text
    
DropDownMenu::DropDownMenu(int w, int text_size_)
:View(w, 1)
{
    init_menu_view(text_size_);
}


void DropDownMenu::init_menu_view(int text_size_) {
    
    
    // Init main view
    resize(get_w(), text_size_ + vertical_padding*2);
    text_size = text_size_;
    selected_entry_idx = 0;
    menu = 0; clicked = false;
    
    set_clear_color(green_color_c);
    fill_with_color(get_clear_color());
    

    // Init menu view
    
    int row_size = (text_size + vertical_padding*2);
    menu = new View(get_w(), (int)(entries.size() ? entries.size() : 1) * row_size);
    
    // always display at least one entry
    menu->draw_onto_self(GUIImage("GUIImages/dropdown_row.bmp"), DispPoint());

    // display full menu
    int i = 0;
    for (list<string>::iterator it = entries.begin(); it != entries.end(); ++it, ++i) {
        
//        menu->attach_subview(new MenuEntry(*it, text_size, *this, i),
//                                                    DispPoint(0, i * row_size));
        
        menu->draw_onto_self(GUIImage("GUIImages/dropdown_row.bmp"),
                             DispPoint(0, i * row_size));

        menu->attach_subview(createTextView(*it, text_size),
                                                    DispPoint(0, i * row_size));

    }
    
    attach_subview(menu, DispPoint());
    
}
void DropDownMenu::update_menu_view() {
    
    // always display at least one entry
    menu->draw_onto_self(GUIImage("GUIImages/dropdown_row.bmp"), DispPoint());

    // display full menu
    int row_size = (text_size + vertical_padding*2);
    
    for (int i = 0; i < entries.size(); ++i) {
        
        menu->draw_onto_self(GUIImage("GUIImages/dropdown_row.bmp"),
                             DispPoint(0, i * row_size));
    }
    
}

    
// Mouse Events. Following three functions all work the same:
//  Returns true if the mouse-event is finished being handled.
//  If returns false, handling will continue up the chain.
//  May optionally call capture_focus() to become the target for keypresses.
bool DropDownMenu::handle_mouse_down(DispPoint coord) {
    
    capture_focus();
    
    if (clicked) {
        return false;
    }
    
    clicked = true;
    pre_click_pos = get_rel_pos();

    resize(get_w(), menu->get_h());
    fill_with_color(get_clear_color());

    int vert_adj = -selected_entry_idx * (text_size + vertical_padding*2); // (up)
    move_subview(menu, DispPoint()); // reset menu subview
    get_parent()->move_subview(this, get_rel_pos() + DispPoint(0,vert_adj));
    
    return true;
}
bool DropDownMenu::handle_mouse_up(DispPoint coord) {

    if (!clicked) {
        return false;
    }
    
    clicked = false;

    // Find clicked row
    int row_size = text_size + vertical_padding*2;
    selected_entry_idx = coord.y / row_size;    
    
    // order matters incase entries is empty to avoid -1
    if (selected_entry_idx >= entries.size())
        { selected_entry_idx = entries.size()-1; }
    if (selected_entry_idx <= 0)
        { selected_entry_idx = 0; }
    
    update_menu_view();
    resize(get_w(), row_size);
    
    int vert_adj = -selected_entry_idx * row_size; // (move subview up)
    move_subview(menu, DispPoint(0, vert_adj)); // move correct entry to visible
    get_parent()->move_subview(this, pre_click_pos);

    lose_focus();
    
    return true;
}
bool DropDownMenu::handle_mouse_motion(DispPoint coord, DispPoint rel_motion) {
    
    if (!clicked) {
        return false;
    }
    
    int row_size = (text_size + vertical_padding*2);

    int row_to_hl = coord.y / row_size;
    
    update_menu_view();
    menu->draw_onto_self(GUIImage("GUIImages/dropdown_row_hl.bmp"),
                         DispPoint(0, row_to_hl * row_size));

    
    return true;
}

    
} // namespace GUI
