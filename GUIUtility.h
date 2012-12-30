
#ifndef GUIUTILITY_H
#define GUIUTILITY_H

#include <string>
#include "stdint.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;


// a simple class for error exceptions - msg points to a C-string error message
struct Error {
	Error(std::string in_msg) : msg(in_msg) { }
	std::string msg;
};

// an exception class for handling Errors not caused by the User. Probably should not output these in a release.
struct Programming_Error {
	Programming_Error(std::string in_msg) : msg(in_msg) { }
	std::string msg;
};


template <typename In, typename Out, typename Pred>
Out copy_if(In first, In last, Out res, Pred p) {
	
	while (first != last) {
		if (p(*first)) {
			*res++ = *first;
		}
		++first;
	}
	return res;
}

inline double square (double x){
	return x*x;
}

class Coord;
struct DispPoint {
	
	DispPoint() : x(0), y(0) { }
	DispPoint(int x_, int y_) : x(x_), y(y_) { }

	
    DispPoint operator+ (DispPoint b) {
        return DispPoint (x + b.x, y + b.y); 
    }
    DispPoint operator- (DispPoint b) {
        return DispPoint (x - b.x, y - b.y); 
    }

	DispPoint(const Coord& coord);
	int x,y;
};


void initSDL(unsigned int flags);
// initializes the SDL with the given flags. Returns -1 if fails and prints error. 0 otherwise.
// also sets SDL to close at exit.




#endif /* GUIUTILITY_H */
