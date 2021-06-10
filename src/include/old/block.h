#include "sprite.h"
#include "legacy_level.h"
#include "pstring.h"
#include "weapons.h"
#include "legacy_graphics.h"
#include "images.h"

#ifndef _BLOCK
#define _BLOCK

class block : public sprite
{
	static string filenames[4];

	legacy::level *current;
	images *cubes;
	//static string imageString[] = {"/owd/images/blue.png","/owd/images/green.png","/owd/images/red.png","/owd/images/yellow.png"};
	//const static imageStore images = new imageStore(imageString);

	bool init;

public:
	const static int width = 18;
	const static int height = 10;

	const static int DEAD = 1;
	const static int MOVING = 2;
	const static int STOPPED = 4;
	const static int BONUS = 8;
	const static int FIRE = 16;
	const static int PERSIST = 32;

	const static int NORMAL_IMG = 0;
	const static int BONUS_IMG = 1;
	const static int FIRE_IMG = 2;
	const static int PERSIST_IMG = 3;

	const static int NORMAL_SCORE = 10;
	const static int BONUS_SCORE = 20;
	const static int FIRE_SCORE = 30;
	const static int PERSIST_SCORE = 50;

	int state;

	int bonusCounter,movementCounter;

	bool allowFire;
	int fireCounter;
	int persistCounter;

public:
	block(legacy::level *l) : sprite() { makeNull(); reset(l); }
	~block() { cleanup(); }

	bool initalised() { return init; }
	void reset(legacy::level *l);

	void update(weapons &w);
	int score();

	void draw(legacy::graphics &g);

protected:
	void makeNull();
	void cleanup();
};

#endif