#include "score.h"
#include "pstring.h"
#include "legacy_graphics.h"

#ifndef _TABLE
#define _TABLE

class table
{
	score **scores;
	
	int currentCharacter;
	string currentName;

	bool up,down;

	bool init;

public:
	const static int maxNames = 10;

	bool inputMode;

	table() { makeNull(); reset(); }
	~table() { cleanup(); }

	bool initalised() { return init; }
	void reset();

	int isHighScore(int points);

	void add(string name,int levelIndex,int points);
	bool update();

	void keyPressed(int e);
	void keyReleased(int e);

	void draw(legacy::graphics g);

protected:
	void makeNull();
	void cleanup();
};

#endif
