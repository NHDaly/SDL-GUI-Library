//
//  Flyweight.cpp
//  Design Patterns
//
//  Created by Nathan Daly on 4/24/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#include "Letter.h"

#include "GameDisplay.h"
#include "GUIUtility.h"

using std::string;
#include <map>
#include <iostream>
using std::map; using std::pair;
using std::cout; using std::endl;

const char* const letters_file_path = "fonts/black/";


void Letter::drawself(SDL_Surface *dest, int x, int y) const{
	
	display_image(image, dest, x, y, 0);
}
int Letter::get_height() const {
	return image->h;
}
int Letter::get_width() const {
	return image->w;
}

Letter* Letter::get_letter(char letter_){
	
	static letter_map_t letters;
	
	Letter* letter(letters[letter_]);
	if (!letter){
		string filename = letters_file_path;
		string spec_char = special_chars(letter_);
		if (spec_char != ""){
			filename += spec_char;
		}
		else if (!isalnum(letter_)) return 0;
		else {
			filename += letter_;
		}
		Letter* result(new Letter);
		filename += ".bmp";
		result->set_image (filename);
		letter = result;		
	}
	return letter;
}

string Letter::special_chars(char letter_){
	
	switch (letter_) {
		case ' ':
			return "_space";
			break;
		case ':':
			return "_colon";
			break;
			
		default:
			return "";
			break;
	}
}

void Letter::set_image(const std::string& filename) { 

	// USE TTF! :D
	
	image = loadBMP(filename); 
	if (!image) {
		string error_msg = "Couldn't load " + filename +": "+ SDL_GetError() +"\n";
		throw GUIError(error_msg.c_str());
	}
}

Letter::Print_Letter::Print_Letter(SDL_Surface *dest, int x_, int y_) 
: x(x_), y(y_), surface(dest)
{ }

void Letter::Print_Letter::operator() (const Letter* letter){
	int width = letter->get_width();
	letter->drawself(surface, x, y);
	x+= width;
}

