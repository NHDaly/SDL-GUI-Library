//
//  Flyweight.cpp
//  Design Patterns
//
//  Created by Nathan Daly on 4/24/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#include "GUIImageCache.h"

#include "GameDisplay.h"
#include "GUIUtility.h"

using std::string;
#include <map>
#include <iostream>
using std::map; using std::pair;
using std::cout; using std::endl;

using GUI::Error;

const SDL_Color transparent = {0xFF, 0, 0xFF, 0};


GUIImage_Cache::~GUIImage_Cache(){
	SDL_FreeSurface(image);
}


void GUIImage_Cache::drawself(SDL_Surface *dest, int x, int y) const{
	
	display_image(image, dest, x, y, 0);
}
int GUIImage_Cache::get_height() const {
	return image->h;
}
int GUIImage_Cache::get_width() const {
	return image->w;
}

GUIImage* GUIImage_Cache::get_image(std::string image_, bool alpha, const SDL_Color& color_key){
	
	static image_map_t images;
	
	GUIImage_Cache* image(images[image_]);
	if (!image){
		image = new GUIImage_Cache; 
        // While this "leaks", the cache is designed to last the entire execution.
        //  So this is okay! :)
		images[image_] = image;
	}
	return &image->image;
}

void GUIImage_Cache::set_image(const std::string& filename) { 
	image = GUIImage(loadBMPAlpha(filename, transparent));
	if (!image) {
		string error_msg = "Couldn't load " + filename +": "+ SDL_GetError() +"\n";
		throw Error(error_msg.c_str());
	}
}

