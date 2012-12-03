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

extern NewGUIView *captured;

void NewGUI_run(NewGUIWindow* window);

#include <vector>
class GUITimer_command;
extern std::vector<GUITimer_command*> timer_commands;

#include "GUITimer.h"

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

// Perform op after interval seconds. Repeat if repeat == true.
template <typename Operation>
void NewGUI_repeat_on_timer(Operation op, double interval, bool repeat = true) {
    
    timer_commands.push_back(create_timer_command(op,interval, repeat));
}



#endif
