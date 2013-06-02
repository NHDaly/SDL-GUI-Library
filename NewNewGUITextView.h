//
//  NewGUITextView.h
//  Deep
//
//  Created by Nathan Daly on 1/4/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#ifndef Deep_NewGUITextView_h
#define Deep_NewGUITextView_h

#include "GUIView.h"

//#include <vector>
#include <list>

struct LetterData {
    
    LetterData(char c_, int size_, SDL_Color color_) 
    : c(c_), size(size_), color(color_)
    { }
    
    char c; // unused when assinging data for a TextView.
    int size;
    SDL_Color color;
    
    bool operator<(const LetterData& rhs) const;
};

//---------------

extern const SDL_Color text_color_black_c;

class NewGUITextView : public GUI::View {
public:
    NewGUITextView(int w, int h, int size = 30, SDL_Color color = text_color_black_c);
    
    virtual void draw();
    
    // Set the size and/or color for the text.
    // If start/end Index are ignored, will apply to all text.
    // Any new text created will always use the last added letter data.
    // (the character is irrelevant in data.)
    void set_letter_data(const LetterData& data,
                         size_t startIdx = 0, size_t endIdx = std::string::npos);
    
    
    void set_text(const std::string& text);
    void add_text(const std::string& text, int index);
    void append_text(const std::string& text);
	std::string get_text() const;

    // NOTE: for index_at_pos, make each letter a subview, and have mouse click 
    // just return its index? Not a bad idea.
    
private:
    typedef std::list<LetterData> letter_ctr_t;
    letter_ctr_t text;
    
    LetterData prev;
    
//    DispPoint pos_at_index(size_t i);
	int index_at_pos(DispPoint pos_);

};


#endif
