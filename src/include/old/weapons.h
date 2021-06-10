#include "weapon.h"
#include "legacy_level.h"
#include "doorways.h"
#include "legacy_graphics.h"

#ifndef _WEAPONS
#define _WEAPONS

class weapons
{
	bool init;

public:
	weapon **weaponsArray;//[];	
	int total,allowed,originalAllowed;

	legacy::level *current;//Level;

public:
	weapons(legacy::level *l,int totalWeapons)
	{
		reset(l,totalWeapons,totalWeapons);
	}

	weapons(legacy::level *l,int totalWeapons,int allowedWeapons)
	{
		reset(l,totalWeapons,allowedWeapons);
	}

	bool initalised() { return init; }
	void reset(legacy::level *l,int totalWeapons,int allowedWeapons);
	void reset();

	void add(int positionX,int positionY);

	void updateNorth();
	void updateSouth();

	void checkCollision(doorways *dw);

	void draw(legacy::graphics &g);

	void inc();
	void dec();

protected:
	void makeNull();
	void cleanup();
};

#endif