//
//  GUIError_Handling_Impl.h
//  GUI Widget Library
//
//  Created by Nathan Daly on 2/18/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#ifndef GUIError_Handling_Impl_h
#define GUIError_Handling_Impl_h

#include <vector>

namespace GUI {
    
// Abstract ErrorCatcher Base Class.
// Derived class will be templated for Error type and Handler type.
class ErrorCatcher {
public:
    typedef std::vector<ErrorCatcher*>::iterator ErrorCatcherIter_t;
    
    virtual void try_catch(ErrorCatcherIter_t begin,
                           ErrorCatcherIter_t end, bool &handled) = 0;
};


// Implementation of ErrorCatcher. Nests try statements for all ErrorCatchers
// passed in, and rethrows the current exception. Then each ErrorCatcher
// attempts to catch during the unravelling.
//
// Error_t : The type of errors that will be caught by handler
// Handler_t : a function or function object that overrides operator()(Error_t);
template <typename Error_t, typename Handler_t>
class ErrorCatcher_Impl : public ErrorCatcher {
public:
    
    // handler_ should be able to call handler_(Error_t)
    // NOTE: handler_ will be copied.
    ErrorCatcher_Impl(const Handler_t &handler_) : handler(handler_) { }
    
    // for each ErrorCatcher in [begin:end), try to handle the error.
    // RESULT: handled will be set to true if any ErrorCatcher successfully
    // handled the current exception.
    // REQUIRES: this must be called from inside a catch{} block.
    virtual void try_catch(ErrorCatcherIter_t begin,
                           ErrorCatcherIter_t end, bool &handled) {
        
        // nest a try block for each ErrorCatcher
        try {
            if (begin == end) { // base case
                throw;
            }
            ErrorCatcher *next = *begin;
            next->try_catch(++begin, end, handled); // unravel until end
        }
        // Each ErrorCatcher gets a chance to try to catch the exception.
        catch(const Error_t &e) { // Will only catch if exception is Error_t
            
            handler(e);     // handler() is only called if Error_t matches.
            handled = true;
            throw;          // continue up the chain.
        }
    }
    
private:
    Handler_t handler;
};
    
} // namespace GUI

#endif
