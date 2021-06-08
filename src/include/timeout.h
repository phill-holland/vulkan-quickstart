#include "legacy_graphics.h"

#ifndef _TIMEOUT
#define _TIMEOUT

class timeout
{
	long startTime,previousTime;
	int flashCounter;
	const static int flashInterval = 20;
	
public:
	long timeRemaining;
	bool red;

public:
	timeout() { reset(); }

	void reset();

	void start(int mm,int ss);
	void start(int ms);
	
	void update();

	bool isFinished();

	void set(int ms);

	void dec(int ms);
	void inc(int ms);

	void draw(legacy::graphics g);
};

#endif