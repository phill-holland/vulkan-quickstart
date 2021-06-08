//#include "legacy_graphics.h"
#include "texture.h"

#ifndef _PLASMA
#define _PLASMA

class plasma : public texture
{
	const static int WIDTH = 163;
	const static int HEIGHT = 36;

private:
	static int horizontialBuffer[];
	static int verticalBuffer[];
	static int horizontialCount[];
	static int verticalCount[];

private:
	int maxWidth, maxHeight;
	int maxBytes;

	int soffset, aoffset;

	//int *pixelBuffer;
	int hBufferLength,vBufferLength;
	
	int green,greenDirection;

	bool init;

public:
	int positionX, positionY;

public:
	plasma() : texture(WIDTH, HEIGHT) { makeNull(); reset(positionX,positionY,WIDTH,HEIGHT); }
	plasma(int w,int h) : texture(w, h) { makeNull(); reset(positionX,positionY,w,h); }
	plasma(int x,int y,int w,int h) : texture(w, h) { makeNull(); reset(x,y,w,h); }

	bool initalised() { return init; }
	void reset(int x,int y,int w,int h);

	void update();//legacy::graphics g);

protected:
	void makeNull();
	void cleanup();
};

#endif