#include "image.h"
#include "sprite.h"
#include "block.h"
#include "weapon.h"
#include "legacy_region.h"
#include "legacy_graphics.h"
#include "images.h"

#ifndef _LINE
#define _LINE

class line : public sprite
{
	static string filenames[1];

	images *imgs;
	//string imageString[] = {"/owd/images/blockline.png"};
	//static imageStore images = new imageStore(imageString);

	int *gateway;//[];
	legacy::region **gatewayRegions;//[];

	const static int gatewayTimeout = 100;

	int hPosition;

	bool init; 

public:
	int widthInPixels,totalColumns;
	int pixelInterval;	

public:
	line(int horizontalPosition,int verticalPosition,int wp,int tc)
	{
		makeNull();
		reset(horizontalPosition,verticalPosition,wp,tc);
	}
	~line() { cleanup(); }

	bool initalised() { return init; }
	void reset(int horizontalPosition,int verticalPosition,int wp,int tc);
	
	void update();
	
	void checkCollision(weapon w);
	
	bool isLive(int column);
	
	void kill(int column) { gateway[column] = 0; }

	bool checkCollision(block b,int column);
	
	void draw(legacy::graphics g);
	
protected:
	void makeNull();
	void cleanup();
};

#endif