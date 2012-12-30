//
//  Singleton.h
//  Deep
//
//  Created by Nathan Daly on 12/15/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Nathan_Singleton_h
#define Nathan_Singleton_h

// Put the following macro inside the class declaration
#define DECLARE_SINGELTON(CLASS_NAME)               \
                                                    \
    public:                                         \
    static CLASS_NAME* get(){                       \
        if (!singleton_ptr)                         \
        singleton_ptr = new CLASS_NAME;             \
        return singleton_ptr;                       \
    }                                               \
                                                    \
    private:                                        \
    static CLASS_NAME * singleton_ptr;              \
    friend class CLASS_NAME_destroyer;              \
                                                    \
    CLASS_NAME() {}                                 \
    ~CLASS_NAME() {}                                \
    CLASS_NAME(const CLASS_NAME&);                  \
    CLASS_NAME& operator= (const CLASS_NAME&);      \
    struct destroyer {                              \
        ~destroyer(){                               \
            delete CLASS_NAME::singleton_ptr;       \
        }                                           \
    };                                              \
    static destroyer the_destroyer;                 


// Put the following macro in the implementation file
#define DEFINE_SINGELTON(CLASS_NAME)                \
                                                    \
CLASS_NAME* CLASS_NAME::singleton_ptr = 0;          \
CLASS_NAME::destroyer CLASS_NAME::the_destroyer;    




#endif
