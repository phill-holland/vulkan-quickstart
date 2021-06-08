#include "images.h"
#include "legacy_graphics.h"
#include "pstring.h"

#ifndef _MENU
#define _MENU

class menu
{
	int hiScoreCounter,beginCounter;

	const static int hiScoreTimeout = 300;
	const static int beginTimeout = 20;

	bool finished,displayScores,displayBegin;

	static string filenames[];
	images *imgs;
	//const static String imageString[] = {"/owd/images/sadistic.png","/owd/images/begin.png","/owd/images/copyright.png"};
	//const static imageStore images = new imageStore(imageString);

	bool init;

public:
	menu()
	{
		makeNull();
		reset();
	}

	bool initalised() { return init; }
	void reset();

	void keyPressed(int e);
	void keyReleased(int e);
	
	bool update();
	void draw(legacy::graphics g);

protected:
	void makeNull();
	void cleanup();
};

#endif