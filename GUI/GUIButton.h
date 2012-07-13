
#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "GUIView.h"
#include "GUIWindow.h"
#include "GUIWin_Ctrl.h"
#include "GUIImage.h"
#include "GameDisplay.h"

#include <string>


template <typename Oper = void(GUIView::*)(const SDL_Event&)>
class GUIButton : public GUIView {
public:
	
	GUIButton(int w, int h, const std::string& image,
			  Oper oper = &GUIView::mouse_click)
	: GUIView(w,h), oper(oper) {
		set_bg(GUIImage(loadBMP(image)));
	}
	GUIButton(int w, int h, const GUIImage& image,
			  Oper oper = &GUIView::mouse_click) :
	GUIView(w,h), oper(oper) {
		set_bg(image);
	}
	
	virtual ~GUIButton() {}
	
	virtual void mouse_click(const SDL_Event& event){
		oper();
	}
	
private:
	Oper oper;	

};



#endif /* GUIBUTTON_H */


