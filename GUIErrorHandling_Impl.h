//
//  Error_Handling_Impl.h
//  GUI Widget Library
//
//  Created by Nathan Daly on 2/18/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#ifndef Error_Handling_Impl_h
#define Error_Handling_Impl_h

#include <vector>

template <typename Exception_t, typename Handler_t>
class ExceptionCatcher_Impl;

namespace GUI {
    
// Abstract ExceptionCatcher Base Class.
// Derived class will be templated for Error type and Handler type.
class ExceptionCatcher {
private:
    
    typedef std::vector<ExceptionCatcher*>::iterator ExceptionCatcherIter_t;
    
    // Recursive function call to iterate through list and try-catch on
    // currently thrown exception.
    virtual void try_catch(ExceptionCatcherIter_t begin,
                           ExceptionCatcherIter_t end, bool &handled) = 0;
    
    template <typename InputIterator>
    friend void call_error_handlers_helper(InputIterator begin,
                                           InputIterator end, bool handled);

    template <typename Exception_t, typename Handler_t>
    friend class ExceptionCatcher_Impl;
};


// Implementation of ExceptionCatcher. Nests try statements for all 
// ExceptionCatchers passed in, and rethrows the current exception.
// Then each ExceptionCatcher attempts to catch during the unravelling.
//
// Exception_t : The type of errors that will be caught by handler
// Handler_t : a function or object that overrides operator()(Exception_t);
template <typename Exception_t, typename Handler_t>
class ExceptionCatcher_Impl : public ExceptionCatcher {
private:
    
    // handler_ should be able to call handler_(Exception_t)
    // NOTE: handler_ will be copied.
    ExceptionCatcher_Impl(const Handler_t &handler_) : handler(handler_) { }
    
    // for each ExceptionCatcher in [begin:end), try to handle the error.
    // RESULT: handled will be set to true if any ExceptionCatcher successfully
    // handled the current exception.
    // REQUIRES: this must be called from inside a catch{} block.
    virtual void try_catch(ExceptionCatcherIter_t begin,
                           ExceptionCatcherIter_t end, bool &handled) {
        
        // nest a try block for each ExceptionCatcher
        try {
            if (begin == end) { // base case
                throw;
            }
            ExceptionCatcher *next = *begin;
            next->try_catch(++begin, end, handled); // unravel until end
        }
        // Each ExceptionCatcher gets a chance to try to catch the exception.
        catch(const Exception_t &e) { // Will only catch if e is of Exception_t
            
            handler(e);     // handler() is only called if Exception_t matches.
            handled = true;
            throw;          // continue up the chain.
        }
    }
    
    template <typename Exc_t, typename Han_t>
    friend ExceptionCatcher* create_error_handler(const Han_t &handler);
    
private:
    Handler_t handler;
};
    
} // namespace GUI

#endif
