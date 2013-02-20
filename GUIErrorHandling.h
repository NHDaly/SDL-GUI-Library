//
//  GUIErrorHandling.h
//  GUI Widget Library
//
//  Created by Nathan Daly on 12/14/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef GUI_ErrorHandling_h
#define GUI_ErrorHandling_h

#include "GUIErrorHandling_Impl.h" // For ErrorCatcher and ErrorCatcher_Impl

namespace GUI {

// Error_t : The type of errors that will be caught by handler
// Handler_t : a function or function object that overrides operator()(Error_t);
// NOTE: handler will be copied.
template <typename Error_t, typename Handler_t>
ExceptionCatcher* create_error_handler(const Handler_t &handler) {
    return new ExceptionCatcher_Impl<Error_t, Handler_t>(handler);
}

// REQUIRES: This function MUST be called from within a catch(){} block!
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
        // Create a vector of handlers
        // (Since try_catch is virtual, it cannot be templated,
        //                                              so it requires a vector.)
        std::vector<ExceptionCatcher*> catchers(begin, end);
        
        catchers.front()->try_catch(++catchers.begin(), catchers.end(), handled);
        
    }
    catch (...) {
        if (!handled) { // Only rethrow if none of the Handlers caught the error.
            throw;
        }
    }
}

} // namespace GUI

#endif /* GUI_ErrorHandling_h */
