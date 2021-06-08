#include "image.h"
#include "pstring.h"

#ifndef _IMAGES
#define _IMAGES

class images
{
	image **data;
	int elements, counter;

	bool init;

public:
	images(int e) { makeNull(); reset(e); }
	images(string *s, int len) { makeNull(); reset(s, len); }

	bool initalised() { return init; }

	void reset(int e);
	void reset(string *s, int len);

	image get(int index);

	void load(string filename,int index);
	void load(string filename);

protected:
	void makeNull();
	void cleanup();
};

#endif
