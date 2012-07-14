


#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H


union SDL_Event;

class GUIController {
	
public:	
	virtual ~GUIController() {}
	virtual void mouse_click_up(const SDL_Event& event);
	virtual void mouse_click_down(const SDL_Event& event);
	virtual void key_up(const SDL_Event& event);
	virtual void key_down(const SDL_Event& event);
	
};



#endif /* GUICONTROLLER_H */


