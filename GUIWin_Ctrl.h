

#ifndef GUI_WINDOW_CONTROLLER_H
#define GUI_WINDOW_CONTROLLER_H

#include "GUIUtility.h"
#include "Ptr_config.h"
#include "GUITimer.h"

#include <iostream>
#include <vector>
#include <list>

class GUIWindow;
class GUIView;
class GUIText_box;
class GUIImage;

class ErrorType {
public:
    virtual ~ErrorType() { }
    
    
};

template <typename Parent>
class ErrorTypeDerived : public Parent, public ErrorType {
    typedef Parent error_type;
};

class GUIWin_Ctrl {
public:
	static GUIWin_Ctrl* get();
	
	
	virtual void run();
	void display_on_screen(const GUIImage& image, DispPoint pos);
	
	
//	void set_responder(GUIVC_shptr_t responder_)
//	{ 
//		event_responder = responder_;
//	}
	void add_responder(GUIVC_shptr_t responder_)
	{ 
		event_responders.push_back(responder_);
	}
	
    // Perform op after interval seconds. Repeat if repeat == true.
	template <typename Operation>
	void repeat_on_timer(Operation op, double interval, bool repeat = true) {
		
		timer_commands.push_back(create_timer_command(op,interval, repeat));
	}
	
	DispPoint get_mouse_offsets(GUIView_shptr_t view);
	
	GUIWindow_shptr_t get_window() {return window; }
	
	void attach(GUIVC_shptr_t view_ctrl);
	void detach(GUIVC_shptr_t view_ctrl);

	void clear_responders() { event_responders.clear(); }
	void clear_repeaters() { 
		for (int i = 0; i < timer_commands.size(); i++){
			delete timer_commands[i];
		}
		timer_commands.clear();
	}


    template <typename T>
    void register_handler(const T& error_type, void(*handler)(const T&));
	
private:
	
	GUIWindow_shptr_t window;
	
	//	GUIVC_shptr_t event_responder;
	std::vector<GUIVC_shptr_t> event_responders;
	
	
	
	struct GUITimer_command {
		GUITimer_command(double interval_) : interval(interval_) {
			timer.start();
		}
		double interval;
		GUITimer timer;
		virtual void execute_command(){}
	};
	
	template <typename Operation>
	class GUITimer_command_impl :public GUITimer_command {
	public:
		GUITimer_command_impl(Operation oper_, double interval_, bool repeat_ = true)
		: GUITimer_command(interval_), oper(oper_), repeat(repeat_), executed(false) { }
		virtual void execute_command(){ 
			if (timer.get_time() > (1000*interval)){
				
				if (!repeat && executed) return;
				
				oper();
				timer.reset();
				executed = true;
			}
		}
	private:
		Operation oper;
		bool repeat;
		bool executed;
	};
	
	template <typename Operation>
	GUITimer_command* create_timer_command(Operation oper, double interval_, bool repeat_ = true){
		return new GUITimer_command_impl<Operation>(oper, interval_, repeat_);
	}
	
	std::vector<GUITimer_command*> timer_commands;
	
    
    struct ErrorToHandler {
        const ErrorType* error;
        void(*handler)(const ErrorType&);
    };
    
    typedef std::list<ErrorToHandler> ErrorsList_t;
    ErrorsList_t errors_to_handlers;
    
    
	//SINGLETON MEMBERS:
	static GUIWin_Ctrl * singleton_ptr; // Only one, class-wide
	
	friend class GUIWin_Ctrl_destroyer;
	
	// no public creation/deletion
	GUIWin_Ctrl(GUIWindow* window_);
	//	GUIWindow();	
	virtual ~GUIWin_Ctrl();
	
	// no copy or assignment allowed
	GUIWin_Ctrl(const GUIWin_Ctrl&);
	GUIWin_Ctrl& operator= (const GUIWin_Ctrl&);
	
	
};



template <typename T>
void GUIWin_Ctrl::register_handler(const T& error_type, void(*handler)(const T&)) {
    
    ErrorToHandler eth;
    eth.error = new ErrorTypeDerived<T>;
    eth.handler = (void(*)(const ErrorType&))handler;
    
    errors_to_handlers.push_back(eth);
}




class GUIWin_Ctrl_destroyer {
	
public:
	~GUIWin_Ctrl_destroyer();
};



#endif /* WINDOW_CONTROLLER_H */
