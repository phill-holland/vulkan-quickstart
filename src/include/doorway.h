#include "sprite.h"
#include "images.h"
#include "colour.h"
#include "pstring.h"

#ifndef _DOORWAY
#define _DOORWAY

class doorway : public sprite
{
	static string filenames[2];
	images *doors;

	bool init;
	//const static String imageString[] = {"/owd/images/entrancebucket.png","/owd/images/exitbucket.png"};
	//const static imageStore images = new imageStore(imageString);

public:
	colour c;
	doorway *linked;
	int id;

	const static int ENTRANCE = 0;
	const static int EXIT = 1;

	int type;

public:
	doorway(int t) : sprite() {	makeNull(); reset(t); }
	doorway(doorway &dw) : sprite() { makeNull(); reset(dw); }
	~doorway() { cleanup(); }

	bool initalised() { return init; }
	void reset(doorway &dw);
	void reset(int t);
	void reset(int x,int y,colour i);

protected:
	void makeNull();
	void cleanup();
};

#endif

