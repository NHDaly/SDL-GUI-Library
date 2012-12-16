//
//  GUILetter.h
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
class NewGUIView; // display image
class SDL_Color; // display image
class _TTF_Font_;
//typedef _TTF_Font_ TTF_Font;

class GUILetter {
	
public:
	
	void drawself(SDL_Surface *dest, int x, int y) const;
	void drawself(NewGUIView *dest, int x, int y) const;
	int get_height() const;
	int get_width() const;
	
	
	static GUILetter* get_letter(char letter_, int size_, SDL_Color color_);
		
	class Print_GUILetter {
	public:
		Print_GUILetter(SDL_Surface *dest, int x_, int y_);
		
		void operator() (const GUILetter* letter);
	private:
		int x, y, width;
		SDL_Surface* surface;
	};
	
	SDL_Surface *image;

private:
	GUILetter() : image (0) {}
	~GUILetter() {}
	
	GUILetter(char letter_, TTF_Font* font, int size_, SDL_Color color_);
	
	typedef std::map<int, TTF_Font*> fonts_map_t;
	typedef std::map<char, std::map<int, std::map<SDL_Color, GUILetter*> > > letter_map_t;
};






#endif /* FLYWEIGHT_H */
