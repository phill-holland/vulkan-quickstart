#include "sprite.h"
#include "legacy_level.h"
#include "legacy_graphics.h"
#include "doorways.h"
#include "images.h"

#ifndef _WEAPON
#define _WEAPON

class weapon : public sprite
{
	legacy::level *current;
	images *imgs;
	//static imageStore images;

	static int constructed;// = 0;

	int hiddenCounter,hiddenTimeout;
	int id;
	bool flip;
	doorways *dwtemp;
	
	bool init;

public:
	static int userWeaponFireSpeedOffset;

	const static int LIVE = 1;
	const static int DEAD = 2;
	const static int HIDDEN = 4;

	int state;

public:
	weapon(legacy::level *l) { makeNull(); reset(l); }

	bool initalised() { return init; }
	void reset(legacy::level *l);

	void updateNorth();
	void updateSouth();

	void update();
	
	void checkCollision(doorways *dw);
	
	void draw(legacy::graphics &g)
	{
		if((state & LIVE)==LIVE) sprite::draw(g);
	}
	
protected:
	void makeNull();
	void cleanup();
};

#endif