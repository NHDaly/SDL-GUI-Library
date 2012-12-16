#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include "Compatibility.h"
#include "GUIUtility.h"

#include <list>
#include <string>


extern const SDL_Color default_color_key_c;
const SDL_Color def_text_c = {255, 255, 255, 0};

const SDL_Rect def_rect_c = {0,0,0,0};



SDL_Surface * loadBMP(std::string file);
// Load a bmp into an image, 
// Returns 0 on failure.
SDL_Surface * loadBMPAlpha(std::string file, SDL_Color color_key = default_color_key_c);
// with alpha support

void display_image (SDL_Surface *src, SDL_Surface *dest, DispPoint pos, bool update, SDL_Rect rect = def_rect_c);
void display_image (SDL_Surface *src, SDL_Surface *dest, int x, int y, bool update, SDL_Rect rect = def_rect_c);

void ShowBMP(std::string file, SDL_Surface *screen, int x, int y, bool update);
// adds a bmp to the screen at x,y). will update that portion of screen iff (update == 1).
void ShowBMP(std::string file, SDL_Surface *screen, SDL_Rect rect, bool update);
//overriden with display rect.

void updateScreen(SDL_Surface *screen);
// updates the entire screen, meaning that anything changed since the last update will now be displayed.

SDL_Surface* createDisplay(SDL_Surface *screen, int width, int height, int bpp,
                           SDL_Color colors[], const int numColors, const Uint32 flags = SDL_SWSURFACE);
// create a Display with the following properties.


void displayToScreen (SDL_Surface *src, int x, int y, bool update, SDL_Rect rect = def_rect_c);
// Displays an image right onto the screen.

void displayTextToScreen (const std::string &a, int x, int y, int size, bool update);
// Displays text onto the screen.

SDL_Surface* createText2 (const std::string &a, int size, SDL_Color textColor = def_text_c);
// create text using sdl font.

SDL_Surface* createText (const std::string &a);

SDL_Surface* create_SDL_Surface(int w, int h);





#endif /* GAMEDISPLAY_H */

