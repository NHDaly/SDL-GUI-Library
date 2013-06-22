//
//  Letter.h
//  Design Patterns
//
//  Created by Nathan Daly on 4/24/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//



#ifndef GUI_LETTER_H
#define GUI_LETTER_H

#include "Ptr_config.h"

#include <string>
#include <map>

class SDL_Surface; // display image

namespace GUI {

class Letter {
	
public:
	
	void drawself(SDL_Surface *dest, int x, int y) const;
	int get_height() const;
	int get_width() const;
	
	
	static Letter* get_letter(char letter_);
		
	class Print_Letter {
	public:
		Print_Letter(SDL_Surface *dest, int x_, int y_);
		
		void operator() (const Letter* letter);
	private:
		int x, y, width;
		SDL_Surface* surface;
	};
	
	SDL_Surface *image;

private:
	Letter() : image (0) {}
	~Letter() {}
	
	Letter(char letter) { 
		std::string filename;
		filename += letter;
		set_image (filename);
	}

	void set_image(const std::string& filename); 


	
	static std::string special_chars(char letter_);
	typedef std::map<char, Letter*> letter_map_t;
};


} // namespace GUI



#endif /* FLYWEIGHT_H */
