#ifndef UTILS_H
#define UTILS_H

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <iostream>


struct VPoint {
	long x,y;
	VPoint(long ix, long iy) : x(ix), y(iy){}
};

struct Point {
	int x,y;
	Point(int ix, int iy) : x(ix), y(iy){}
};




#endif
