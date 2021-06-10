#include "image.h"
#include "legacy_region.h"
#include "legacy_graphics.h"

#ifndef _SPRITE
#define _SPRITE

class sprite
{
	bool init;

public:
	image picture;

	int positionX,positionY;	
	int state;
	
public:

	sprite(image i) { makeNull(); set(i); }
	sprite() { makeNull(); reset(); }
	~sprite() { cleanup(); }

	bool initalised() { return init; }
	void reset();

	void set(image i);
	
	int width();
	int height();

	bool isCollision(legacy::region *r);	
	bool isCollision(sprite *s);
	
	void draw(legacy::graphics &g)
	{
		g.drawImage(picture,positionX,positionY,0);	
	}

protected:
	bool isInside(int x,int y);
	
	void makeNull() { }
	void cleanup() { }
};

#endif