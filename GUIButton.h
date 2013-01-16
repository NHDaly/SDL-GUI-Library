
#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "GUIView.h"
#include "GUIWindow.h"
#include "GUIWin_Ctrl.h"
#include "GUIImage.h"
#include "GameDisplay.h"
#include "SDL/SDL.h"

#include <string>


class DoNothing {
public:
    void operator()() { }
};

template <typename Oper = DoNothing>
class GUIButton : public GUIView {
    // There are two ways to create a new button's action:
    // 1) Provide an Operation in the Template,
    // 2) Override its mouse_click() function.
    
public:
	
	GUIButton(int w, int h, const std::string& image,
			  Oper oper = Oper())
	: GUIView(w,h), oper(oper), is_pressed(false) {
		set_bg(GUIImage(loadBMP(image)));
	}
	GUIButton(int w, int h, const GUIImage& image,
			  Oper oper = &GUIView::mouse_click) :
	GUIView(w,h), oper(oper) {
		set_bg(image);
	}
		
	virtual void mouse_click(const SDL_Event& event){

        if (event.button.type == SDL_MOUSEBUTTONDOWN) {
            is_pressed = true;
            set_bg(GUIImage("GUIImages/button2.bmp"));
        }
        // Only perform event on mouse release.
        else if (is_pressed) {
            oper();
            is_pressed = false;
        }
	}
	
private:
	Oper oper;	
    bool is_pressed;
};



#endif /* GUIBUTTON_H */


