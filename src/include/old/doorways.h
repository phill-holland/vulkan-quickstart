#include <random>
#include "doorway.h"
#include "pstring.h"
#include "images.h"
#include "colour.h"
#include "legacy_graphics.h"

#ifndef _DOORWAYS
#define _DOORWAYS

class doorways
{
	static std::mt19937_64 generator;

	static string filenames[2];
	images *doors;
	//const static string imageString[] = {"/owd/images/entranceline.png","/owd/images/exitline.png"};
	//const static images images = new images(imageString);

	const static int ENTRANCE = 0;
	const static int EXIT = 1;

	const static int cycleTimeout = 200;
	int cycleCounter;
	int cycleDelay;

	bool init;

public:
	int positionX,positionY;

	static colour uniqueColours[7];// = {colour(0,0,255),colour(0,255,0),colour(255,0,0),colour(255,255,0),colour(0,255,255),colour(255,0,255),colour(255,255,255)};

	doorway **entrances;//[];
	doorway **exits;//[];

	int widthInPixels,pixelInterval,totalDoors;
	int seperatorHeight;

public:
	doorways(int x,int y,int td,int wp,int sh)
	{
		makeNull();
		reset(x,y,td,wp,sh);
	}
	~doorways() { cleanup(); }

	bool initalised() { return init; }	
	void reset(int x,int y,int td,int wp,int sh);

	void update();

	void enableCycle();

	void shuffleEntrances();
	void shuffleExits();

	int findEntrance(int id);
	int findExit(int id);

	void draw(legacy::graphics g);

protected:
	void makeNull();
	void cleanup();
};

#endif