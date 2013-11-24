//
//  Error_Handling_Impl.h
//  GUI Widget Library
//
//  Created by Nathan Daly on 2/18/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#ifndef GUI_Error_Handling_Impl_h
#define GUI_Error_Handling_Impl_h

#include <vector>

template <typename Exception_t, typename Handler_t>
class ExceptionHandler_Impl;

namespace GUIExceptionHandling {
    
// Abstract ExceptionHandler Base Class.
// Derived class will be templated for Error type and Handler type.
class ExceptionHandler {
private:
    
    typedef std::vector<ExceptionHandler*>::iterator ExceptionHandlerIter_t;
    
    // Recursive function call to iterate through list and try-catch on
    // currently thrown exception.
    virtual void try_catch(ExceptionHandlerIter_t begin,
                           ExceptionHandlerIter_t end, bool &handled) = 0;
    
    template <typename InputIterator>
    friend void call_exception_handlers_helper(InputIterator begin,
                                               InputIterator end, bool handled);

    template <typename Exception_t, typename Handler_t>
    friend class ExceptionHandler_Impl;
};


// Implementation of ExceptionHandler. Nests try statements for all 
// ExceptionHandlers passed in, and rethrows the current exception.
// Then each ExceptionHandler attempts to catch during the unravelling.
//
// Exception_t : The type of errors that will be caught by handler
// Handler_t : a function or object that overrides operator()(Exception_t);
template <typename Exception_t, typename Handler_t>
class ExceptionHandler_Impl : public ExceptionHandler {
// everything is private, so that ExceptionHandler_Impl isn't created anywhere
// but from create_exception_handler.
private:
    
    // handler_ should be able to call handler_(Exception_t)
    // NOTE: handler_ will be copied.
    ExceptionHandler_Impl(const Handler_t &handler_) : handler(handler_) { }
    
    // for each ExceptionHandler in [begin:end), try to handle the error.
    // RESULT: handled will be set to true if any ExceptionHandler successfully
    // handled the current exception.
    // REQUIRES: this must be called from inside a catch{} block.
    virtual void try_catch(ExceptionHandlerIter_t begin,
                           ExceptionHandlerIter_t end, bool &handled) {
        
        // nest a try block for each ExceptionHandler
        try {
            if (begin == end) { // base case
                throw;
            }
            ExceptionHandler *next = *begin;
            next->try_catch(++begin, end, handled); // unravel until end
        }
        // Each ExceptionHandler gets a chance to try to catch the exception.
        catch(const Exception_t &e) { // Will only catch if e is of Exception_t
            
            handler(e);     // handler() is only called if Exception_t matches.
            handled = true;
            throw;          // continue up the chain.
        }
    }
    
    // Friend this because of the private constructor
    template <typename Exc_t, typename Han_t>
    friend ExceptionHandler* create_exception_handler(const Han_t &handler);
    
private:
    Handler_t handler;
};
    
} // namespace GUIExceptionHandling

#endif /* GUI_Error_Handling_Impl_h */
