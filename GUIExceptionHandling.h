//
//  ExceptionHandling.h
//  GUI Widget Library
//
//  Definitions for the Exception Handling functions:
//    ExceptionHandler* create_exception_handler(const Handler_t &handler);
//    void call_exception_handlers(InputIterator begin, InputIterator end);
//
//  Created by Nathan Daly on 12/14/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//


#ifndef GUI_Exception_Handling_h
#define GUI_Exception_Handling_h

#include "GUIExceptionHandling_Impl.h" // Contains details irrelevant to clients

namespace GUIExceptionHandling {

// Exception_t : The type of the exceptions that will be caught by handler
// Handler_t : a function or object that overrides operator()(Exception_t);
// NOTE: handler will be copied.
template <typename Exception_t, typename Handler_t>
ExceptionHandler* create_exception_handler(const Handler_t &handler) {
    return new ExceptionHandler_Impl<Exception_t, Handler_t>(handler);
}

// Loop through error handlers and handle any errors. If no handler matches
//  the error, it will be rethrown out of the function.
// REQUIRES: This function MUST be called from within a catch(){} block!
template <typename InputIterator>
void call_exception_handlers(InputIterator begin, InputIterator end) {
    call_exception_handlers_helper(begin, end, false);
}

template <typename InputIterator>
void call_exception_handlers_helper(InputIterator begin,
                                    InputIterator end, bool handled) {
    
    try {
        // Create a vector of handlers
        // (Since try_rethrow_catch is virtual, it cannot be templated,
        //                                             so it requires a vector.)
        std::vector<ExceptionHandler*> catchers(begin, end);
        
        catchers.front()->try_rethrow_catch(++catchers.begin(),
                                            catchers.end(), handled);
        
    }
    catch (...) {
        if (!handled) { // Only rethrow if none of the Handlers caught the error.
            throw;
        }
    }
}

} // namespace GUIExceptionHandling

#endif /* GUI_Exception_Handling_h */
