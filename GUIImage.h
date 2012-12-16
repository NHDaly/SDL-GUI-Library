


#ifndef GUIIMAGE_H
#define GUIIMAGE_H

#include <string>
#include <map>

#include "Compatibility.h"


extern const SDL_Color default_color_key_c;


class GUIImage {
public:
	GUIImage() : sdl_impl(0) {}
	GUIImage(int w, int h, bool alpha = false, const SDL_Color& color_key =default_color_key_c);
	explicit GUIImage(SDL_Surface*);
	
	GUIImage(std::string filename, bool alpha = false, const SDL_Color& color_key =default_color_key_c);
	
	~GUIImage();
	
	GUIImage(const GUIImage&);
	GUIImage& operator= (const GUIImage&);
	
	static GUIImage* get_image(std::string filename, bool alpha = false, const SDL_Color& color_key =default_color_key_c);
	
	static GUIImage create_blank(int w, int h);
	static GUIImage create_filled(int w, int h, SDL_Color color);
    static GUIImage create_clear(int w, int h);
	static GUIImage create_outline(int w, int h, int width, SDL_Color color);
    static GUIImage create_copy(const SDL_Surface*); // copy constructor

    
	operator SDL_Surface*() const {return sdl_impl;}
	SDL_Surface* operator->() const {return sdl_impl;}
	
	void display(SDL_Surface* dest, int x, int y);
	void clear();
	
	int getw() const { return sdl_impl->w; }
	int geth() const { return sdl_impl->h; }
	
	Uint32 get_Alpha() const;
	
private:
	SDL_Surface* sdl_impl;
	
    bool is_alpha;
    SDL_Color alpha_color;
	
	typedef std::map<std::string, GUIImage*> image_map_t;
};


#endif /* GUIIMAGE_H */


