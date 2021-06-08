#include "colour.h"
#include "surface.h"

#ifndef _TEXTURE
#define _TEXTURE

class texture : public surface
{				
	bool init;

public:
	unsigned long width,height;
	unsigned long length;

	colour *data;

public:
	texture(unsigned long w, unsigned long h) { makeNull(); reset(w, h); }
	texture() { makeNull(); width = 0UL; height = 0UL; length = 0UL; init = false; }

	bool initalised() { return init; }
	void reset(unsigned long w, unsigned long h);

	void set(unsigned long x, unsigned long y, colour &c);

	colour get(unsigned long x, unsigned long y);
	colour get(float x, float y);
	
	int getWidth() { return (int)width; }
    int getHeight() { return (int)height; }

	void render(int x, int y, surface *src);

	void clear(colour &c);
		
	unsigned char *get();

	void line(unsigned long minX, unsigned long minY, unsigned long maxX, unsigned long maxY, colour &c);
	void circle(unsigned long x, unsigned long y, unsigned long radius, colour &c);
	void ellipse(unsigned long x, unsigned long y, unsigned long w, unsigned long h, colour &c, bool fill);

	void copy(texture &src);

protected:
	void makeNull();
	void cleanup();
};

#endif