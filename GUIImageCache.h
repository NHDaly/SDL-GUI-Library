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
#include "GUIImage.h"

#include <string>
#include <map>

class SDL_Surface; // display image

class GUIImage_Cache {
	
public:
	
	void drawself(SDL_Surface *dest, int x, int y) const;
	int get_height() const;
	int get_width() const;
	
	
	static GUIImage* get_image(std::string image_, bool alpha = 0, const SDL_Color& color_key =default_color_key_c);
			

	mutable GUIImage image;

private:
	GUIImage_Cache() : image () {}
//	GUIImage_Cache(std::string image_) { 
//		set_image (image_);
//	}
	~GUIImage_Cache();


	void set_image(const std::string& filename); 


	typedef std::map<std::string, GUIImage_Cache*> image_map_t;
    static image_map_t images;

};






#endif /* FLYWEIGHT_H */
