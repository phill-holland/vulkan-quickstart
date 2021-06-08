#include "pstring.h"

#ifndef _SCORE
#define _SCORE

class score
{
public:
	string name;
	int points,levelIndex;

public:
	score()
	{
		name = "";
		points = 0;
		levelIndex = 1;
	}

	score(string n, int l, int s)
	{
		name = n;
		points = s;
		levelIndex = l;
	}
};

#endif