//
//  NewGUIErrorHandling.h
//  Deep
//
//  Created by Nathan Daly on 12/14/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_NewGUIErrorHandling_h
#define Deep_NewGUIErrorHandling_h

class ErrorCatcher {
public:
    virtual void operator()() = 0;
    bool handled() {
        return was_handled;
    }
protected:
    bool was_handled; 
};


template <typename Error_t, typename Handler_t>
class ErrorCatcher_Impl : public ErrorCatcher {
public:
    
    ErrorCatcher_Impl(const Handler_t &handler_) 
    : handler(handler_)
    { }
    
    virtual void operator()() {
        was_handled = false;
        
        try {
            throw;
        }
        catch(const Error_t &e) {
            handler(e);
            was_handled = true;
            throw;
        }
    }
    
    
private:
    Handler_t handler;
};

template <typename Error_t, typename Handler_t>
ErrorCatcher* create_error_handler(const Handler_t &handler) {
    return new ErrorCatcher_Impl<Error_t, Handler_t>(handler);
}

// REQUIRES: This function MUST be called from within a catch(...){} block!
// Loop through error handlers and handle any errors. If no handler matches
//  the error, it will be rethrown out of the function.
template <typename InputIterator>
void call_error_handlers(InputIterator begin, InputIterator end) {
    call_error_handlers_helper(begin, end, false);
}

template <typename InputIterator>
void call_error_handlers_helper(InputIterator begin,
                                 InputIterator end, bool handled) {
    
    if (begin == end) { 
        if (!handled) {
            throw;
        }
        return;
    }
    
    try {
        (**begin)();
    }
    catch (...) {
        if (!handled)
            handled = (*begin)->handled();
        call_error_handlers_helper(++begin, end, handled);
    }
}



#endif
