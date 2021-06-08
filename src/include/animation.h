#include "sprite.h"
#include "legacy_graphics.h"
#include "coordinate.h"

#ifndef _ANIMATION
#define _ANIMATION

class animation
{
	int totalSprites;

	bool init;

public:
	sprite **Sprites;
	int currentIndex;

	int positionX;
	int positionY;

public:
	//animatedSprite() { totalSprites = 0; currentIndex = 0; }
	animation(int count) { makeNull(); reset(count); }
	~animation() { cleanup(); }

	bool initalised() { return init; }
	void reset(int count);

	void setPosition(coordinate pos);
	
	void inc();

	void dec();

	bool isCollision(animation &as);
	bool isCollision(sprite &b);

	int width();
	int height();

	void draw(legacy::graphics g);

protected:
	void makeNull();
	void cleanup();
};

#endif