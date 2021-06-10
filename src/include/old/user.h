#include "pstring.h"
#include "images.h"
#include "weapons.h"
#include "score.h"

#ifndef _USER
#define _USER

class user
{
	// have functions here to detect the collision of a weapon with a certain object .. 
	// void doorwayCollision() ... or have functions within weapon ...

	static string filenames[3];
	images *imgs;

	//static string imageString[] = {"/owd/images/user.png","/owd/images/gray.png","/owd/images/tinyuser.png"};
	//const static imageStore images = new imageStore(imageString);

	bool left,right,fire;

	int movementTimeout;
	int movementCounter;

	legacy::level *current;

	bool init;

public:
	int positionX,positionY,columnPosition;

	int widthInPixels,totalColumns;
	int pixelInterval;

	const static int width = 18;
	const static int height = 10;

	weapons *Weapons;

	int score;
	int lives;

	const static int weaponTimeout = 50; // repeatDelay
	const static int totalWeapons  = 3;

	int weaponCounter;

	bool red;

	user(legacy::level *l,int horizontalPosition,int verticalPosition,int wp,int tc)
	{
		makeNull();
		reset(l,horizontalPosition,verticalPosition,wp,tc);
	}
	~user() { cleanup(); }

	void reset(legacy::level *l,int horizontalPosition,int verticalPosition,int wp,int tc);
	
	void reset();
	
	void update();
	
	void draw(legacy::graphics &g);
	void drawStatistics(legacy::graphics &g);
	
private:
	void inc();
	void dec();
	
public:
	bool isCollision(weapons ws);
	
	void keyPressed(int e);
	void keyReleased(int e);

protected:
	void makeNull();
	void cleanup();
};

#endif