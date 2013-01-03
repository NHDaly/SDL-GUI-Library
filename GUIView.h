//
//  Controller.h
//  SDL_Word_Processor
//
//  Created by Nathan Daly on 5/2/12.
//  Copyright 2012 Lions Entertainment. All rights reserved.
//

#ifndef GUI_VIEW_H
#define GUI_VIEW_H


#include "Ptr_config.h"
#include "GUIUtility.h"
#include "GUIImage.h"

#include <vector>

//class GUIWindow;
const SDL_Rect full_rect_c = {0,0,0,0};

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

class Letter;
class Letter_Disp_Obj;

class SDL_Surface;
class GUIView : public std::tr1::enable_shared_from_this<GUIView> {
public:
	GUIView(int w_ = SCREEN_WIDTH, int h_ = SCREEN_HEIGHT, std::string bg_image = "images/bg.bmp");
	virtual ~GUIView();
	
	virtual void draw_onto_view(SDL_Surface* source, DispPoint pos, SDL_Rect rect = full_rect_c, bool update = 0);
	
	virtual void draw_bg();
	virtual void display();
	virtual void clear() { }
	
	virtual void mouse_click(const SDL_Event& event);
	virtual void key_down(const SDL_Event& event);
	
	int get_h() const {return screen.geth(); }
	int get_w() const {return screen.getw(); }
	
	virtual void resize(double scale);
	virtual void resize(double w, double h);
	virtual void rotate(double degrees); // (negative is counter-clockwise)
	
	void set_bg(std::string filename);
	void set_bg(GUIImage bg) { background = bg; }
	//protected:
	//	GUIWindow* window;
	GUIImage screen;
	
	GUIImage background;
	
	void attach_to_view_controller(GUIVC_shptr_t view_controller);
	
	GUIVC_shptr_t view_controller;
	
};



class Selectable_view : public GUIView {
public:
	
	Selectable_view(int w_ = 200, int h_ = 200);	
	~Selectable_view();
	
	virtual void display();
	virtual void mouse_click(SDL_Event event);
	
private:
	SDL_Surface* background;
	int color;
	SDL_Color colors[4];
	
};



#endif /* GUI_VIEW_H */
