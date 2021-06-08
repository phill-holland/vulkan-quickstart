#include "effect.h"
#include "legacy_graphics.h"

#ifndef _EFFECTS
#define _EFFECTS

class effects
{
	bool init;

public:
	effect **bonusArray;//[];
	
	int widthInPixels,totalColumns;
	int pixelInterval;
	int positionX,positionY,offsetX,offsetY;

public:
	effects(int horzPos,int vertPos,int widPix,int totCol)
	{
		makeNull();
		reset(horzPos,vertPos,widPix,totCol);
	}

	bool initalised() { return init; }
	void reset(int horzPos,int vertPos,int widPix,int totCol);
	
	void add(int doorwayIndex,int type);
	
	void update();
	
	void draw(legacy::graphics g);

protected:
	void makeNull();
	void cleanup();
};

#endif