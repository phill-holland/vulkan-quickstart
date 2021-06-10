#include "blocks.h"
#include "legacy_graphics.h"
#include "line.h"
#include <random>

#ifndef _COLUMNS
#define _COLUMNS

class columns
{
	static std::mt19937_64 generator;

	bool init;

public:
	int positionX;

	legacy::level *current;

	int widthInPixels,totalColumns,bottomLine;
	int pixelInterval;

	blocks **blocksArray;

public:
	columns(legacy::level *l,int x,int bl,int wp,int tc) { makeNull(); reset(l,x,bl,wp,tc); }
	~columns() { cleanup(); }
	
	bool initalised() { return init; }
	void reset(legacy::level *l,int x,int bl,int wp,int tc);
	void reset();

	void update(line &bl,user &u,doorways **dw,int dwlen,effects &bEffects,timeout &ct);

	void checkCollision(doorways &dw);

	bool isFull();

	void draw(legacy::graphics &g);

private:
	void generate();
	
protected:
	void makeNull();
	void cleanup();
};

#endif