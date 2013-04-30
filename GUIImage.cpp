
#include "GUIImage.h"
#include "GameDisplay.h"

#include "GUIUtility.h"
#include "GUIImageCache.h"

#include <iostream>
using std::cout; using std::endl;

using std::string;

const SDL_Color clear_color = {0xFF, 0, 0xFF, 0xFF};


GUIImage::GUIImage(int w, int h, bool alpha, const SDL_Color& color_key)
{
	
	SDL_Surface* temp = create_SDL_Surface(w, h);
	if (!temp){
		throw GUIError("Could not create GUIImage. Not enough memory.");
	}	
	sdl_impl = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	if (!sdl_impl){
		throw GUIError("Could not create GUIImage. Not enough memory.");
	}
    
    if (alpha){
		//Map the color key
		Uint32 colorkey = SDL_MapRGB(sdl_impl->format,
                                     color_key.r, color_key.g, color_key.b);
		//Set all pixels of color R 0xFF, G 0, B 0xFF to be transparent
		SDL_SetColorKey(sdl_impl, SDL_SRCCOLORKEY, colorkey );
	}

}

GUIImage::GUIImage(SDL_Surface* surface)
:is_alpha(false)
{
	
    if (surface == 0) throw GUIError("Cannot form a GUIImage from NULL surface!");
	sdl_impl = surface;
}


GUIImage::GUIImage(string filename, bool alpha, const SDL_Color& color_key){
//	cout <<" constructing an image " + filename << endl;
	
	sdl_impl = GUIImage_Cache::get_image(filename)->sdl_impl; 
	
	sdl_impl = loadBMP(filename);
	
	if (alpha){
		//Map the color key
		alpha_color = SDL_MapRGB(sdl_impl->format,
                                     color_key.r, color_key.g, color_key.b);
        set_alpha(alpha_color);
	}
}


GUIImage::~GUIImage(){
	if (sdl_impl)
		SDL_FreeSurface( sdl_impl );
}


GUIImage::GUIImage(const GUIImage& image_){
	
    if (image_.sdl_impl == 0) throw GUIError("Cannot form a GUIImage from bad image");
	SDL_Surface* temp = create_SDL_Surface(image_.getw(), image_.geth());
	if (!temp){
		throw GUIError("Could not copy GUIImage. Not enough memory.");
	}	
	sdl_impl = SDL_DisplayFormat(temp);
	if (!sdl_impl){
		throw GUIError("Could not copy GUIImage. Not enough memory.");
	}


    Uint32 colorkey = image_.sdl_impl->format->colorkey;

    Uint8 alpha = image_.sdl_impl->format->alpha;
    
    
    // First fill background with the clear color, then display and re-clear.
    SDL_FillRect(sdl_impl, 0, colorkey);
    
	display_image(image_.sdl_impl, sdl_impl, 0, 0, 1);
    if (image_.is_alpha /*|| alpha != 0*/) {
        //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
        SDL_SetColorKey(sdl_impl, SDL_SRCCOLORKEY, colorkey);
    }
}
GUIImage& GUIImage::operator= (const GUIImage& image_){
	
	GUIImage temp(image_);
	std::swap(sdl_impl, temp.sdl_impl);
	return *this;
}

GUIImage* GUIImage::get_image(std::string filename, bool alpha, const SDL_Color& color_key){
	static image_map_t images;
	
	GUIImage* image = images[filename];
	if (!image){
		images[filename] = new GUIImage(filename,alpha,color_key);
	}
	return images[filename];
}

void GUIImage::set_alpha(Uint32 alpha) {
    
    alpha_color = alpha;
    //Set all pixels of color R 0xFF, G 0, B 0xFF to be transparent
    SDL_SetColorKey(sdl_impl, SDL_SRCCOLORKEY, alpha_color );
    is_alpha = true;
}


Uint32 GUIImage::get_Alpha() const{
    
	return SDL_MapRGBA( sdl_impl->format, clear_color.r, clear_color.g, clear_color.b, clear_color.unused);
}
SDL_Color GUIImage::get_clear_color() const {
        
	return clear_color;
}

bool GUIImage::point_is_color(int x, int y, SDL_Color color) const {
    
    if (x < 0 || y < 0 || x >= getw() || y >= geth()) 
        return false;
    
    Uint32 pixel = getpixel(sdl_impl, x, y);
    if (pixel == SDL_MapRGB(sdl_impl->format, color.r, color.g, color.b)) {
        return true;
    }
    return false;
}


GUIImage GUIImage::create_blank(int w, int h){
	
	GUIImage result;
	
	result.sdl_impl = create_SDL_Surface(w, h);
	if (!result.sdl_impl){
		throw GUIError("Could not create blank GUIImage. Not enough memory.");
	}	
	return result;
}

GUIImage GUIImage::create_filled(int w, int h, SDL_Color color){
	
	GUIImage blank (w , h);
	
	Uint32 colorkey = SDL_MapRGBA( blank->format, color.r, color.g, color.b, color.unused);
	SDL_FillRect(blank, 0, colorkey);
	
	return blank;
}


GUIImage GUIImage::create_clear(int w, int h){
	
	GUIImage hollow (w , h);
	
	Uint32 colorkey = SDL_MapRGBA( hollow->format, clear_color.r, clear_color.g, clear_color.b, clear_color.unused);
	SDL_FillRect(hollow, 0, colorkey);
	SDL_SetColorKey( hollow, SDL_SRCCOLORKEY, colorkey );
	
	return hollow;
	
}

GUIImage GUIImage::create_outline(int w, int h, int width, SDL_Color color){
	const SDL_Color clear_color = {0, 0, 0, 0xFF};
	
	GUIImage dispBox(w, h);
	SDL_FillRect(dispBox, 0, SDL_MapRGB(dispBox->format, color.r, color.g, color.b));
	
	GUIImage hollow (w - width*2, h - width*2);
	
	Uint32 colorkey = SDL_MapRGBA( dispBox->format, clear_color.r, clear_color.g, clear_color.b, clear_color.unused);
	SDL_FillRect(hollow, 0, colorkey);
	display_image(hollow, dispBox, 3, 3, 1);
	SDL_SetColorKey( dispBox, SDL_SRCCOLORKEY, colorkey );
	
	return dispBox;
}
GUIImage GUIImage::create_copy(const SDL_Surface* surface){
    
    GUIImage temp1(const_cast<SDL_Surface*>(surface));
    GUIImage temp2(temp1); // make a copy

    
    temp1.sdl_impl = 0; // get the surface out of a GUIImage so it isn't destructed.
    return temp2;
}



void GUIImage::display(SDL_Surface* dest, int x, int y){
	
	//	SDL_Rect dest_rect = {x, dest->h - y, sdl_impl->w, sdl_impl->h};
	SDL_Rect dest_rect = {x, y, sdl_impl->w, sdl_impl->h};
	
	SDL_BlitSurface(sdl_impl, 0, dest, &dest_rect);
	//	if (update) SDL_UpdateRects(dest, 1, &dest_rect);
	
}

void GUIImage::clear(){
	GUIImage temp(create_clear(getw(), geth()));
	std::swap(sdl_impl, temp.sdl_impl);
}




