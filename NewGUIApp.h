//
//  NewGUIApp.h
//  Deep
//
//  Created by Nathan Daly on 11/27/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_NewGUIApp_h
#define Deep_NewGUIApp_h

#include "NewGUIWindow.h"
#include "NewGUIErrorHandling.h"
#include "GUITimer.h"

#include <vector>
#include <list>
#include <set>


class NewGUIApp {
public:
  	static NewGUIApp* get();
  
    void run(NewGUIWindow* window);
    
    // Perform op after interval seconds. Repeat if repeat == true.
    template <typename Operation>
    void repeat_on_timer(Operation op, double interval, bool repeat = true);

    template <typename Error_t, typename Handler_t>
    void register_error_handler(const Handler_t &handler);

    void give_focus(NewGUIView* view) { captured_focus.insert(view); }
    bool has_focus(NewGUIView* view) { return captured_focus.count(view) != 0; }
    void release_focus(NewGUIView* view) { captured_focus.erase(view); }
    
private:
    typedef std::set<NewGUIView*> view_list_t;
    view_list_t captured_focus;

    
    typedef std::list<ErrorCatcher*> error_handler_list_t;
    error_handler_list_t handler_list;
    
    
    class GUITimer_command;

    std::vector<GUITimer_command*> timer_commands;
    
    struct GUITimer_command {
        GUITimer_command(double interval_) : interval(interval_) {
            timer.start();
        }
        double interval;
        GUITimer timer;
        virtual void execute_command(){}
    };
    
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

//SINGLETON MEMBERS:
	static NewGUIApp * singleton_ptr; 
	
	friend class NewGUIApp_destroyer;
	
	// no public creation/deletion
	NewGUIApp() {}
	~NewGUIApp() {}
	
	// no copy or assignment allowed
	NewGUIApp(const NewGUIApp&);
	NewGUIApp& operator= (const NewGUIApp&);
	
    
    struct NewGUIApp_destroyer {
        ~NewGUIApp_destroyer();
    };
    static NewGUIApp_destroyer the_NewGUIApp_destroyer;
};

// Perform op after interval seconds. Repeat if repeat == true.
template <typename Operation>
void NewGUIApp::repeat_on_timer(Operation op, double interval, bool repeat) {
    
    timer_commands.push_back(create_timer_command(op,interval, repeat));
}

template <typename Error_t, typename Handler_t>
void NewGUIApp::register_error_handler(const Handler_t &handler) {
    handler_list.push_back(create_error_handler<Error_t>(handler));
}




#endif
