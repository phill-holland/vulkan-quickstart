#include <random>
#include "block.h"
#include "weapons.h"
#include "legacy_graphics.h"
#include "line.h"
#include "user.h"
#include "doorways.h"
#include "effects.h"
#include "timeout.h"
#include "legacy_level.h"

#ifndef _BLOCKS
#define _BLOCKS

class blocks
{
	static std::mt19937_64 generator;

	legacy::level *current;

	int virtualBottom;
	int virtualBottomPerm;
	int addDelay,addCounter;
	bool delay;

	int columnIndex;

	const static int totalWeapons = 10;

	bool init; 

public:
	block **blockArray;

	int totalBlocks;
	int offsetX,bottomOffsetY;

	weapons *Weapons;

	int blockDropSpeedOffset;

public:
	blocks(legacy::level *l,int ci,int columnPixel,int rowPixel,int numberOfBlocks)
	{
		makeNull();
		reset(l,ci,columnPixel,rowPixel,numberOfBlocks);
	}
	~blocks() { cleanup(); }

	void reset(legacy::level *l,int ci,int columnPixel,int rowPixel,int numberOfBlocks);
	
	bool initalised() { return init; }
	void reset();

	void update(line &bl,user &u,doorways **dw,int dwlen,effects &bEffects,timeout &ct);

	void add();
	bool isFull();

	void draw(legacy::graphics &g);

protected:
	void makeNull();
	void cleanup();
};

#endif