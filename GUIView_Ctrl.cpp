
#include "GUIView_Ctrl.h"
#include "GUIView.h"
#include "GUIWin_Ctrl.h"
#include "GUIWindow.h"

#include "Compatibility.h"
#include <iostream>
using namespace std;

GUIView_Ctrl::GUIView_Ctrl(GUIView_shptr_t view)
: view(view) 
{ 
	//	view->attach_to_view_controller(shared_from_this());
//	GUIWin_Ctrl::get()->get_window()->attach(view, DispPoint(0,0));
}
GUIView_Ctrl::~GUIView_Ctrl() {
	cout << "GUIView_Ctrl Destructor" << endl;
}


void GUIView_Ctrl::mouse_click_up(const SDL_Event& event){
	
	
	
}
void GUIView_Ctrl::mouse_click_down(const SDL_Event& event){
	
	
	
}
void GUIView_Ctrl::mouse_motion(const SDL_Event& event){
	
}

void GUIView_Ctrl::key_up(const SDL_Event& event){
	
	
	
}
void GUIView_Ctrl::key_down(const SDL_Event& event){
	
	
}


