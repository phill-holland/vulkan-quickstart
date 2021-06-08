#include "pstring.h"
#include "colour.h"
#include "surface.h"

#ifndef _IMAGE
#define _IMAGE

namespace legacy
{
    class graphics;
};

class image : public surface
{
    int width, height;
    int pixelsize;//, stride;
    unsigned long length;

    unsigned char *buffer;

    bool init;

//private:

    
public:
    image() { makeNull(); init = false; }
    image(string filename) { makeNull(); reset(filename); }
    ~image() { cleanup(); }

    bool initalised() { return init; }
    void reset(string filename);

    int getWidth() { return width; }
    int getHeight() { return height; }

	void set(unsigned long x, unsigned long y, colour &c) { }
//	colour get(unsigned long x, unsigned long y) { return colour(); }
	
    colour get(unsigned long x, unsigned long y);

    void render(int x, int y, surface *src) { }

public:
    legacy::graphics *getGraphics();// { return NULL; }

    static image *createImage(string filename);
    static image *createImage(int width, int height);// { return NULL; }

    //static image *createImageOld(string filename);
protected:
    void makeNull();
    void cleanup();
};

#endif