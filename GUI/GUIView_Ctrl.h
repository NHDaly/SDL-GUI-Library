

#ifndef GUIView_Ctrl_H
#define GUIView_Ctrl_H

#include "GUIController.h"
#include "Ptr_config.h"

class GUIView_Ctrl : public GUIController
//, public std::tr1::enable_shared_from_this<GUIView_Ctrl>
{
public:
	
	GUIView_Ctrl(GUIView_shptr_t view);
	virtual ~GUIView_Ctrl();
	
	
	virtual void mouse_click_up(const SDL_Event& event);
	virtual void mouse_click_down(const SDL_Event& event);
	virtual void mouse_motion(const SDL_Event& event);
	virtual void key_up(const SDL_Event& event);
	virtual void key_down(const SDL_Event& event);
	virtual void joystick_event(const SDL_Event& event) { }
	
protected:
	GUIView_shptr_t view;
	
};



#endif /* GUIView_Ctrl_H */

