#include "legacy_graphics.h"
#include "image.h"

#ifndef _EFFECT
#define _EFFECT

class effect
{
	int currentCellX,currentCellY;
	int currentX,currentY;

	int width,height;

	image *picture;
	legacy::graphics pg;

	bool stop;
	//int[] tempArray;
	int *tempArray;

	const static int bonusTimeout = 100;
	int bonusCounter;

	bool init;

public:
	int type;

	const static int DEAD = 0;
	const static int POINTS = 1;
	const static int RATEINC = 2;
	const static int RATEDEC = 3;
	const static int CYCLE = 4;
	const static int LIFE = 5;
	const static int LEVEL = 6;
	const static int TIMEINC = 7;
	const static int TIMEDEC = 8;
	// ***
	const static int FASTER = 9;
	const static int UNLIMITED = 10;	
	// ***

	const static int total = 8;

	int positionX,positionY;
	int offsetX,offsetY;
	
public:
	effect() { makeNull(); reset(); }
	~effect() { cleanup(); }

	bool initalised() { return init; }
	void reset();

	void start(int t);
	void update();

	void draw(legacy::graphics g);

protected:
	void makeNull();
	void cleanup();
};

#endif