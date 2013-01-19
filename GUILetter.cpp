//
//  Flyweight.cpp
//  Design Patterns
//
//  Created by Nathan Daly on 4/24/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#include "GUILetter.h"

#include "GameDisplay.h"
#include "GUIUtility.h"

using std::string;
#include <map>
#include <iostream>
using std::map; using std::pair;
using std::cout; using std::endl;

const char* const letters_file_path = "fonts/black/";


#include "GUIWin_Ctrl.h"
#include "GUIWindow.h"
#include "NewGUIView.h"
#include "GUIImage.h"

void GUILetter::drawself(SDL_Surface *dest, int x, int y) const{
	
	display_image(image, dest, x, y, 0);
}
void GUILetter::drawself(NewGUIView *dest, int x, int y) const{
	
    dest->draw_onto_self((GUIImage&)image, DispPoint(x,y));
}
int GUILetter::get_height() const {
	return image->h;
}
int GUILetter::get_width() const {
    // SUPER HACKY
    if (image->w == 0) { return 5; } // for space ' '
	return image->w;
}

bool operator< (const SDL_Color& a, const SDL_Color& b) {
	if (a.r == b.r) {
		if (a.g == b.g) {
			return a.b < b.b;
		}
		else return a.g < b.g;
	}
	else return a.r < b.r;
}

GUILetter* GUILetter::get_letter(char letter_, int size_, SDL_Color color_){
	
	static letter_map_t letters;
	static fonts_map_t fonts;
	
	GUILetter* &letter = letters[letter_][size_][color_];
	if (!letter){

		TTF_Font* &font = fonts[size_];
		if (!font){
			font = TTF_OpenFont("GUIFonts/arial.ttf", size_);
            if (!font) throw GUIError("Couldn't open font!");
		}
		letter = new GUILetter(letter_, font, size_, color_);
	}
	return letter;
}

GUILetter::GUILetter(char letter_, TTF_Font* font, int size_, SDL_Color color_){

    string ch(1,letter_);
	image = TTF_RenderText_Solid(font, ch.c_str(), color_);
}

GUILetter::Print_GUILetter::Print_GUILetter(SDL_Surface *dest, int x_, int y_) 
: x(x_), y(y_), surface(dest)
{ }

void GUILetter::Print_GUILetter::operator() (const GUILetter* letter){
	int width = letter->get_width();
	letter->drawself(surface, x, y);
	x+= width;
}

