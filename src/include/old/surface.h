#include "colour.h"

#ifndef _SURFACE
#define _SURFACE

class surface
{
public:
	virtual void set(unsigned long x, unsigned long y, colour &c) = 0;
	virtual colour get(unsigned long x, unsigned long y) = 0;
	
	virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    virtual void render(int x, int y, surface *src) = 0;
};

#endif
/*

	void set(unsigned long x, unsigned long y, colour &c);

	colour get(unsigned long x, unsigned long y);
	
	int getWidth() { return (int)width; }
    int getHeight() { return (int)height; }

    void render(int x, int y, texture *src);

*/

// can be used as base for texture and image and canvas

