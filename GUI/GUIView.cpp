//
//  Controller.cpp
//  SDL_Word_Processor
//
//  Created by Nathan Daly on 5/2/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#include "GUIView.h"

#include "Letter.h"
#include "GameDisplay.h"
#include "GUIWin_Ctrl.h"
#include "GUIWindow.h"

#include <algorithm>
#include <iostream>
using std::for_each;

const SDL_Color bg_color_c = {120,120,210,0};


GUIView::GUIView(int w_, int h_, std::string bg_image)
:background(w_, h_), screen(w_,h_)
{ 
	SDL_PixelFormat *fmt = background->format;
	Uint32 background_color = SDL_MapRGB(fmt, bg_color_c.r, bg_color_c.g, bg_color_c.b);
	SDL_FillRect(background, 0, background_color);
	//	SDL_SetColorKey( background, SDL_SRCCOLORKEY, background_color );
	
}
GUIView::~GUIView()
{
	std::cout << "destructing GUIView" << std::endl;
}


void GUIView::draw_onto_view(SDL_Surface* source, DispPoint pos, SDL_Rect rect, bool update) {
	
	if (!source) {
		throw Error("Bad source image");
	}
	//	display_image(source, screen, x, screen.geth() - y, update);	
	display_image(source, screen, pos.x, pos.y, update, rect);	
}

void GUIView::draw_bg() {
	draw_onto_view(background, DispPoint(0,0));
}
void GUIView::display() {
	
	//	GUIImage temp(get_w(), get_h());
	//	display_image(background, temp, 0, 0, 0);
	//	display_image(screen, temp, 0, 0, 0);
	//	display_image(temp, screen, 0, 0, 0);
	//	updateScreen(screen);
	
	//	display_image(screen, dest, view_box->pos.x, view_box->pos.y, 0);
	
	//	SDL_SetColorKey( screen, SDL_SRCCOLORKEY, screen.get_Alpha() );
}


void GUIView::mouse_click(const SDL_Event& event){
//	std::cout <<"GUIVIEW mouse_click" << std::endl;
	//	handle_mouse_down(DispPoint(event.button.x, event.button.y));
}
void GUIView::key_down(const SDL_Event& event){
	
	//	handle_key_down(event.key.keysym.sym);
}


void GUIView::resize(double scale){
	
	//	SDL_Surface* temp = create_SDL_Surface(screen->w * scale, screen->h * scale);
	
	
}

void GUIView::rotate(double degrees){
	
	
	
}

void GUIView::set_bg(std::string filename){
	background = *GUIImage::get_image(filename, false);
}

void attach_to_view_controller(GUIVC_shptr_t view_controller){
	view_controller = view_controller;
}




Selectable_view::Selectable_view(int w_, int h_) 
: GUIView(w_,h_), color(0)
{
	for (int i = 0; i < 5; i++){
		colors[i].r = i*20;
		colors[i].g = i*3;
		colors[i].b = i*40;
	}
	
	background = create_SDL_Surface(screen.getw(), screen.geth());
	if (!background) throw Error("Couldn't load background. Not enough mem.");
	SDL_PixelFormat *fmt = background->format;
	Uint32 background_color = SDL_MapRGB(fmt, colors[color].r, colors[color].g, colors[color].b);
	SDL_FillRect(background, 0, background_color);
}

Selectable_view::~Selectable_view() {
	SDL_FreeSurface(background);
}

void Selectable_view::mouse_click(SDL_Event event){
	
	color = (color +1) % 5;
	SDL_PixelFormat *fmt = background->format;
	Uint32 background_color = SDL_MapRGB(fmt, colors[color].r, colors[color].g, colors[color].b);
	SDL_FillRect(background, 0, background_color);
	
}

void Selectable_view::display() {
	
	display_image(background, screen, 0, 0, 0);
	
	//	GUIView::display();
}


