
#include "GameDisplay.h"
#include "GUIWin_Ctrl.h"
#include "GUIWindow.h"
#include "GUIUtility.h"
#include "Compatibility.h"
#include <cctype>
#include <vector>
#include <iostream>
using std::isspace;
using std::vector;
using std::list;
using std::string;
using std::cout; using std::endl;

const int c_char_image_length = 17;
const int c_char_image_height = 19;

const SDL_Color default_color_key_c = {0xFF, 0, 0xFF, 0};


SDL_Surface* load_letter(char ch_, string text_image_file, string extension);


void updateScreen(SDL_Surface *screen){
	
	if (SDL_Flip( screen ) == -1){
		throw Error("Failed Screen Flip");
	}
}

SDL_Surface* createDisplay(SDL_Surface *screen, int width, int height, int bpp, SDL_Color colors[], const int numColors){
    /* Create a display surface with a grayscale palette */
    
	if (width <= 0 || height <= 0 || bpp <= 0){
		throw Error("non-positive values in createDisplay not allowed!");
	}
    
    /* Fill colors with color information */
    for(int i = 0; i < numColors; i++){
        colors[i].r = i;
        colors[i].g = i;
        colors[i].b = i;
    }
    
    /* Create display */
    screen=SDL_SetVideoMode(width, height, bpp, SDL_SWSURFACE);
    if(!screen){
        throw Error("Couldn't set video mode: \n" + string(SDL_GetError()));
    }
    
    /* Set palette */
    SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);
	
    return screen;
}


SDL_Surface * loadBMP(string file) {    
	//Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = SDL_LoadBMP( file.c_str() );
	
    //If nothing went wrong in loading the image
    if( !loadedImage ) {
		string error_msg = "Couldn't load " + file +": "+ SDL_GetError() +"\n";
		throw Error(error_msg);
	}

	//The optimized image that will be used
    SDL_Surface* optimizedImage = SDL_DisplayFormat( loadedImage );
	
	//Free the old image
	SDL_FreeSurface( loadedImage );
    
	if (!optimizedImage) {
		string error_msg = "Couldn't optimize " + file +": "+ SDL_GetError() +"\n";
		throw Error(error_msg);
	}
	
    //Return the optimized image
    return optimizedImage;
}
SDL_Surface * loadBMPAlpha(string file, SDL_Color color_key)
{
    SDL_Surface *image = loadBMP(file);
        
	//Map the color key
	Uint32 colorkey = SDL_MapRGB( image->format, color_key.r, color_key.g, color_key.b);
	//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
	SDL_SetColorKey( image, SDL_SRCCOLORKEY, colorkey );
	
	return image;
}

void ShowBMP(string file, SDL_Surface *screen, int x, int y, bool update)
{
    SDL_Surface *image;
    
	image = loadBMP(file);
    if ( image == NULL ) {
        return;
    }
    
    display_image(image, screen, x, y, update);
	SDL_FreeSurface(image);
}
void ShowBMP(string file, SDL_Surface *screen, SDL_Rect rect, bool update)
{
	ShowBMP(file, screen, rect.x, rect.y, update);
}




void display_image (SDL_Surface *src, SDL_Surface *dest, DispPoint pos, bool update, SDL_Rect rect){
	
	display_image (src, dest, pos.x, pos.y, update, rect);
}
void display_image (SDL_Surface *src, SDL_Surface *dest, int x, int y, bool update, SDL_Rect rect){
	
	SDL_Rect dest_rect = {x, y, rect.w, rect.h};

//	if (rect.w != def_rect_c.w && rect.h != def_rect_c.h){
//		SDL_BlitSurface(src, &rect, dest, &dest_rect);		
//	}

	SDL_BlitSurface(src, 0, dest, &dest_rect);
	if (update) SDL_UpdateRects(dest, 1, &dest_rect);
}

void displayToScreen (SDL_Surface *src, int x, int y, bool update, SDL_Rect rect) {
	
	display_image(src, GUIWin_Ctrl::get()->get_window()->screen, x, y, update, rect);
}

void displayTextToScreen (const string &a, int x, int y, int size, bool update){
	
	static TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", size);
	
	if (!font) throw Error("Couldn't load font: arial.ttf");
	
	SDL_Color textColor = {255, 255, 255};
	
	SDL_Surface* message = TTF_RenderText_Solid(font, a.c_str(), textColor);

	display_image(message, GUIWin_Ctrl::get()->get_window()->screen, x, y, update);

	SDL_FreeSurface(message);
}

SDL_Surface* createText (const string &a){
	
	string alphaImage = "fonts/";
	string fileExtension = ".bmp";
	
	if (a.size() == 0) {
		return 0;
	}
	
	vector<SDL_Surface *> letters;
	int x = 0;
	
	string filePath = alphaImage;
	for (int i = 0; i < a.size(); i++) {
		SDL_Surface *letter = load_letter(a[i], alphaImage, fileExtension);
		if (!letter) {
			return 0;
		}
		x += letter->w;
		letters.push_back(letter);
	}
	
	SDL_Surface *surface = create_SDL_Surface(x, letters[0]->h);
	x = 0;
	for (int i = 0; i < letters.size(); i++) {
		
		
		
		SDL_Rect rect = {x, 0, letters[i]->w, letters[i]->h};
		SDL_BlitSurface(letters[i], NULL, surface, &rect);
		
		x += letters[i]->w;
		SDL_FreeSurface(letters[i]);
	}
	
	return surface;
}


SDL_Surface* load_letter(char ch_, string text_image_file, string extension){
	
	SDL_Surface *letter;
	string filePath = text_image_file;

	if (isspace(ch_)) filePath += "_space" + extension;
	else if (ch_ == ':') filePath += "_colon" + extension;
	else filePath += ch_ + extension;
	
	letter = SDL_LoadBMP(filePath.c_str());
	
	return letter;
}

SDL_Surface* create_SDL_Surface(int w, int h){

    /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
	 as expected by OpenGL for textures */
    SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;
	
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
	
    surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32,
                                   rmask, gmask, bmask, amask);
    if(surface == NULL) {
        cout << "CreateRGBSurface failed: \n" + string(SDL_GetError()) << endl;
		return 0;
    }
	SDL_Surface* updatedimage = SDL_DisplayFormat(surface);
	if(!updatedimage) {
        cout << "updateimage in CreateRGBSurface failed: \n" + string(SDL_GetError());
		return 0;
    }

	return updatedimage;
}

