#include "legacy_level.h"
#include "legacy_graphics.h"
#include "timeout.h"
#include "line.h"
#include "effects.h"
#include "columns.h"
#include "user.h"
#include "timeout.h"

#ifndef _ENGINE
#define _ENGINE

class engine
{
	legacy::level **levels;//[] = {new level1() };//,new level2(),new level3(),new level4(),new level5()};
	int currentLevel;

	bool finished;
	bool currentLevelFinished;

	bool deathPause;

	int deathPauseCounter;
	const static int deathPauseTimeout = 70;

	bool startLevel;
	int startLevelCounter;
	const static int startLevelTimeout = 70;

	doorways **dw;
	columns *bc;
	line *bl;
	user *u;
	timeout *ct;
	effects *bEffects;

	bool init;

public:
	engine() { makeNull(); reset(0); }
	~engine() { cleanup(); }

	bool initalised() { return init; }
	void reset(int theLevel);
	
	bool initObjects();
	void updateObjects();
	void drawObjects(legacy::graphics g);

	bool update();

	void keyPressed(int e);
	void keyReleased(int e);

	void draw(legacy::graphics g);

protected:
	void makeNull();
	void cleanup();
};

#endif