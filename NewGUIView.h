//
//  NewGUIView.h
//  Deep
//
//  Created by Nathan Daly on 9/10/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_NewGUIView_h
#define Deep_NewGUIView_h

#include <list>

#include "GUIUtility.h"

/// @todo TEMP
#include <iostream>

class NewGUIView {
public:
    
    NewGUIView(NewGUIView* parent_, DispPoint pos_ = DispPoint()) 
    :changed(false), parent(0), background(0) 
    { if (parent_) parent_->attach_subview(this, pos_); }
    
    // Redraw any children that have changed onto self.
    void update() {
        
        if (!has_changed()) return;
        
        std::list<GUISubview*>::iterator child;
        for(child = children.begin(); child != children.end(); child++) {
            
            if (!(*child)->view->has_changed()) continue;

            (*child)->view->update();
            
            /// @todo seperate drawing from updating, so don't double update. That's a waste of processing power.
            draw_onto_self((*child)->view, (*child)->pos);
        }
    }

    // Renders an image onto this screen. Also marks all ancestors as modified.
    void draw_onto_self(NewGUIView* view, DispPoint pos);

    void attach_subview(NewGUIView* view, DispPoint pos) {
        if (view->parent)
            throw Error("Candidate vew is already a subview of another view.");
        
        children.push_back(new GUISubview(view, pos));
        view->parent = this;
    }
    
    // Will be true if an image has been drawn onto this view or a subview.
    bool has_changed() const { return changed; }
    
    /// === DEBUG ===
    void print_children() {
        std::cout << this << std::endl;

        for(std::list<GUISubview*>::iterator child = children.begin(); child != children.end(); child++) {
            std::cout << (*child)->view << " " << (*child)->pos.x << (*child)->pos.y << std::endl;
        }
    }
    
private:
    bool changed;
    
    struct GUISubview {
        GUISubview(NewGUIView* view_, DispPoint pos_) : view(view_), pos(pos_) {}
        NewGUIView* view;
        DispPoint pos;
    };
    
    NewGUIView* parent;
    std::list<GUISubview*> children;
    
    NewGUIView* background;

};



#endif
