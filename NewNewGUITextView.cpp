//
//  NewNewGUITextView.cpp
//  Deep
//
//  Created by Nathan Daly on 1/4/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include "NewNewGUITextView.h"
#include "GameDisplay.h"

#include <iostream>
#include <map>
using std::string;
using std::map;
using std::make_pair;

//---------------

class LetterCache {
public: 
    static const GUIImage& get_image(const LetterData& data);
    
};

//---------------

const SDL_Color text_color_black_c = {0,0,0,0};

const SDL_Color clear_color_c = {0xFF, 0, 0xFF};
const SDL_Color white_color_c = {0xFF, 0xFF, 0xFF};


NewNewGUITextView::NewNewGUITextView(int w, int h, int size, SDL_Color color)
:NewGUIView(w,h), prev('a', size, color)
{
    draw_onto_self(GUIImage::create_filled(w, h, clear_color_c), DispPoint());
    set_clear_color(clear_color_c);    
}

void NewNewGUITextView::draw() {
    
    DispPoint pos(0,0);

    letter_ctr_t::iterator it;
    for (it = text.begin(); it != text.end(); ++it) {
        const GUIImage& image = LetterCache::get_image(*it);
        
        draw_onto_self(image, pos);
        
        pos.x += image.getw();
        if (pos.x + image.getw() >= get_w()) { 
            pos.x = 0;
            pos.y += image.geth();
        }
    }
}


static bool operator<(const SDL_Color& a, const SDL_Color& b) {
    if (a.r < b.r) return true;
    else if (a.r == b.r && a.b < b.b) return true;
    else if (a.b == b.b && a.g < b.g) return true;

    else return false;
}

bool LetterData::operator<(const LetterData& rhs) const {
    
    if (c < rhs.c) return true;
    else if (c == rhs.c && size < rhs.size) return true;
    else if (size == rhs.size && color < rhs.color) return true;
    
    else return false;
}


void NewNewGUITextView::set_letter_data(const LetterData& data,
                     size_t startIdx, size_t endIdx) {
    
    prev = data;
    
    if (endIdx == string::npos) endIdx = text.size();

    letter_ctr_t::iterator it = text.begin();
    for (int i = 0; i < startIdx; i++) ++it;   // advance to correct position.
  
    for (size_t i = startIdx; i < endIdx; ++i, ++it) { 
        it->size = data.size;
        it->color = data.color;
    }
}


void NewNewGUITextView::set_text(const std::string& text_){
	
    text.clear();
    add_text(text_, 0);
}
void NewNewGUITextView::add_text(const std::string& text_, int index){
	
    if (index > text.size()) {
        throw Error("Cannot add text: Index is out of bounds.");
    }
            
    letter_ctr_t new_text;
    for (int i = 0; i < text_.size(); ++i) { 
        new_text.push_back(LetterData(text_[i], prev.size, prev.color));
    }
    
    letter_ctr_t::iterator it = text.begin();
    for (int i = 0; i < index; i++) ++it;   // advance to correct position.
    
    text.insert(it, new_text.begin(), new_text.end());

    draw();
}

void NewNewGUITextView::append_text(const std::string& text_){
	add_text(text_, text.size());
}

std::string NewNewGUITextView::get_text() const {
	
    string result;
    
    letter_ctr_t::const_iterator it;
    for (it = text.begin(); it != text.end(); ++it) {
        result.append(1, it->c);
    }
        
    return result;
}


const GUIImage& LetterCache::get_image(const LetterData& data) {
    
    static map<LetterData, SDL_Surface*> letters;
    static map<int, TTF_Font*> fonts;
    
    if (fonts.count(data.size) == 0) {
        fonts[data.size] = TTF_OpenFont("fonts/arial.ttf", data.size);
    }
    TTF_Font *font = fonts[data.size];
    
    if (letters.count(data) == 0) {
        string ch(1, data.c);
        SDL_Surface* text = TTF_RenderText_Solid(font, ch.c_str(),
                                                 data.color);
        
        letters.insert(make_pair(data, text));
    }
    // todo As it is, TTF_RenderText creates Alpha'd images, but the GUIImage copy constructed images have a completely black background.
    
    return (GUIImage&)letters[data]; // Avoid the copy constructor.
}

                                                 
