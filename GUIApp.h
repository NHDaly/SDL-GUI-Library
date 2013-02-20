//
//  GUIApp.h
//  Deep
//
//  Created by Nathan Daly on 11/27/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_GUIApp_h
#define Deep_GUIApp_h

#include "GUIErrorHandling.h"
#include "GUITimer.h"
#include "GUIUtility.h"

#include <vector>
#include <list>
#include <set>

#include <SDL/SDL.h> // To ensure it is #included by main.


// Throw an instance of GUIQuit to safely tell the application to exit.
// (This is the same as calling GUIApp::quit())
class GUIQuit {};
class GUIWindow;

class GUIController;

struct GUITimer_command {
    GUITimer_command(double interval_) : interval(interval_) {
        timer.start();
    }
    double interval;
    GUITimer timer;
    virtual void execute_command(){}
};


class GUIApp {
public:
  	static GUIApp* get();
    
    void set_framerate_cap(int fps_cap_) { fps_cap = fps_cap_; cap_frame_rate = true;}
    void disable_framerate_cap() { cap_frame_rate = false; }
    
    void run(GUIWindow* window);
    
//    struct GUITimer_command;

   // Perform op after interval seconds. Repeat if repeat == true.
    // returns a ptr to the timer command, so that it may be canceled later.
    template <typename Operation>
    GUITimer_command*  repeat_on_timer(Operation op, double interval, bool repeat = true);
    // Perform op after interval seconds. Repeat if repeat == true.
	void cancel_timer_op(GUITimer_command* op);
    
    // When any code executed within the run() loop throws an instance of Error_t,
    // any Handler_t's registered for that Error_t will be called.
    template <typename Error_t, typename Handler_t>
    void register_error_handler(const Handler_t &handler);

    // Provides a Controller with the ability to receive mouse/keyboard input.
    //  (Views receive mouse input by defualt when hovered over.)
    void give_focus(GUIController* view) { captured_focus.insert(view); }
    bool has_focus(GUIController* view) { return captured_focus.count(view) != 0; }
    void release_focus(GUIController* view) { captured_focus.erase(view); }
    
    
    // Equivalent to a user clicking the "x" or pressing cmd-q.
    void quit() { throw GUIQuit(); }
    

    DispPoint get_screen_size();    
private:
    
    GUIWindow* window;
    
    int fps_cap;
    bool cap_frame_rate;
    
    typedef std::set<GUIController*> view_list_t;
    view_list_t captured_focus;

    
    // vector because it's fast to iterate through!
    typedef std::vector<GUI::ErrorCatcher*> error_handler_list_t;
    error_handler_list_t handler_list;
    
    

    typedef std::vector<GUITimer_command*> timer_cmnds_list_t;
    timer_cmnds_list_t timer_commands;
    
    
    template <typename Operation>
    class GUITimer_command_impl : public GUITimer_command {
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
    
    timer_cmnds_list_t::iterator next_timer_cmd;
    void cycle_timer_commands();

//SINGLETON MEMBERS:
	static GUIApp * singleton_ptr; 
	
	friend class GUIApp_destroyer;
	
	// no public creation/deletion
	GUIApp();
	
	// no copy or assignment allowed
	GUIApp(const GUIApp&);
	GUIApp& operator= (const GUIApp&);
	
    
    struct GUIApp_destroyer {
        ~GUIApp_destroyer();
    };
    static GUIApp_destroyer the_GUIApp_destroyer;
};

// Perform op after interval seconds. Repeat if repeat == true.
template <typename Operation>
GUITimer_command* GUIApp::repeat_on_timer(Operation op, double interval, bool repeat) {
    
    GUITimer_command* command = create_timer_command(op,interval, repeat);
    timer_commands.push_back(command);
    
    next_timer_cmd = --timer_commands.end();
    

    return command;
}



template <typename Error_t, typename Handler_t>
void GUIApp::register_error_handler(const Handler_t &handler) {
    handler_list.push_back(GUI::create_error_handler<Error_t>(handler));
}





#endif
