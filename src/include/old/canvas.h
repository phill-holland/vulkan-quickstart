#include "thread.h"
#include "semaphore.h"
#include "regions.h"
#include "display.h"
#include "opengl.h"
//#include "scoring.h"
#include "colour.h"
#include "font.h"
#include "image.h"
#include "texture.h"
#include "surface.h"

#ifndef _CANVAS
#define _CANVAS

//namespace graph
//{
class canvas : public thread, public surface, public regions::context
{
public:
	const static unsigned long WIDTH = 640UL, HEIGHT = 480UL;

public:
	void background(thread *bt);

private:
	colour *buffer;

	display::context *context;

	unsigned int width, height;
	//float minimum, maximum, highest;

	//float difference;

	graphics::font *font;

	semaphore::token token;
	
	bool init;

public:
	canvas() { makeNull(); reset(); } //min, max, high); }
	~canvas() { cleanup(); }

	bool initalised() { return init; }
	void reset();//float min = 0.0f, float max = 0.0f, float high = 0.0f);

	bool title(string source) { return context->title(source); }
	//void overall(float value) { difference = value; }
	
	void clear();

	void set(unsigned long x, unsigned long y, colour &c) { }
	colour get(unsigned long x, unsigned long y) { return colour(); }
	
	int getWidth() { return (int)width; }
    int getHeight() { return (int)height; }
	//bool save(string filename);

public:
	void render(void *linear);
	//void render(int x, int y, image *src);
	void render(int x, int y, surface *src);

protected:
	void clear(void *linear);
	
//public:
//	void copy(scoring::scoring *source);

protected:
	void makeNull();
	void cleanup();
};
//};

#endif