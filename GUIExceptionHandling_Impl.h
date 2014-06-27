//
//  ExceptionHandling_Impl.h
//  GUI Widget Library
//
//  Implementation for the ExceptionHandler class.
//
//  Created by Nathan Daly on 2/18/13.
//  Referenced from .
//

#ifndef GUI_Exception_Handling_Impl_h
#define GUI_Exception_Handling_Impl_h

#include <vector>

namespace GUIExceptionHandling {

// Forward Declaration for friending
template <typename Exception_t, typename Handler_t>
class ExceptionHandler_Impl;


// Abstract ExceptionHandler Base Class.
// Derived class will be templated for Error type and Handler type.
class ExceptionHandler {
private:    // Everything is private so that this can only be used as a base
            //  class for the ExceptionHandler_Impl class.
    
    
    // A virtual function cannot be templated, so we require that try_catch,
    //  below, use a vector::iterator.
    typedef std::vector<ExceptionHandler*>::iterator ExceptionHandlerIter_t;
    
    // Recursive function call to iterate through list and try-catch on
    // currently thrown exception.
    virtual void try_rethrow_catch(ExceptionHandlerIter_t begin,
                                   ExceptionHandlerIter_t end, bool &handled)=0;
    
    
    // This is the public interface for interacting with ExceptionHandlers
    template <typename InputIterator>
    friend void call_exception_handlers_helper(InputIterator begin,
                                               InputIterator end, bool handled);
    
    // The only actual derived class that will use this class's functions.
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
private:    // Everything is private, so that ExceptionHandler_Impl isn't
    //  created anywhere but from create_exception_handler().
    
    
    // handler_ should be a callable entity s.t. handler_(Exception_t) is valid.
    // NOTE: handler_ will be copied.
    ExceptionHandler_Impl(const Handler_t &handler_) : handler(handler_) { }
    
    // For each ExceptionHandler in [begin:end), try to handle the error.
    // RESULT: handled will be set to true if any ExceptionHandler successfully
    // handled the current exception.
    // REQUIRES: this must be called from inside a catch{} block.
    virtual void try_rethrow_catch(ExceptionHandlerIter_t begin,
                                   ExceptionHandlerIter_t end, bool &handled) {
        
        // nest a try block for each ExceptionHandler
        try {
            if (begin == end) { // base case
                throw;
            }
            ExceptionHandler *next = *begin;
            next->try_rethrow_catch(++begin, end, handled); // unravel until end
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

#endif /* GUI_Exception_Handling_Impl_h */
