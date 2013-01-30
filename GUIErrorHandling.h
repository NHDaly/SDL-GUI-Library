//
//  NewGUIErrorHandling.h
//  Deep
//
//  Created by Nathan Daly on 12/14/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_NewGUIErrorHandling_h
#define Deep_NewGUIErrorHandling_h

#include <vector>

class ErrorCatcher {
public:
    typedef std::vector<ErrorCatcher*>::iterator ErrorCatcherIter_t;
    virtual void operator()(ErrorCatcherIter_t begin,
                            ErrorCatcherIter_t end, bool &handled) = 0;
protected:
};


template <typename Error_t, typename Handler_t>
class ErrorCatcher_Impl : public ErrorCatcher {
public:
    
    ErrorCatcher_Impl(const Handler_t &handler_) 
    : handler(handler_)
    { }
    
    virtual void operator()(ErrorCatcherIter_t begin,
                            ErrorCatcherIter_t end, bool &handled) {
        
        // nest a try block for each ErrorCatcher
        try {
            if (begin == end) { // base case
                throw;
            }
            ErrorCatcher *next = *begin;
            (*next)(++begin, end, handled); // unravel until end
        }
        // Each ErrorCatcher gets a chance to try to catch.
        catch(const Error_t &e) {
            
            handler(e);     // handler() is only called if Error_t matches.
            handled = true;
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
    
    try {
        // Create the handlers 
        std::vector<ErrorCatcher*> catchers(begin, end);
        
        (*catchers.front())(++catchers.begin(), catchers.end(), handled);
        
    }
    catch (...) {
        if (!handled) { // Only rethrow if none of the Handlers caught the error.
            throw;
        }
    }
}



#endif
