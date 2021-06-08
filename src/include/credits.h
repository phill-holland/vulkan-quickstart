#include "image.h"
#include "pstring.h"
#include "legacy_graphics.h"

#ifndef _CREDITS
#define _CREDITS

class credits
{
	image *leftWipe,*rightWipe;
	int currentPage;
	int delayTimeout;
	int delayCounter;

	bool wipeScreen,delayScreen;

	int leftXOffset;
	int rightXOffset;

	int gridX,gridY;

	static string pageOne[];// = {"SADISTIC"," "," ","DEVELOPED"," "," ","AT"," "," ","OWD STUDIOS"," "," ","ENGLAND"};
	static string pageTwo[];// = {"ORIGINAL IDEA"," "," ","PHILL HOLLAND"," "," ","GAME DESIGN"," "," ","PAUL PARROT"," "," ","LEVEL DESIGN "," "," ","ALAN AARVARK"};
	static string pageThree[];// = {"GRAPHICS"," "," ","PHILL HOLLAND"," "," ","SOUND"," "," ","GARY GOPHER"," "," ","MUSIC"," "," ","STEVE BABOON"};
	static string pageFour[];// = {"PROGRAMMING"," "," ","PHILL HOLLAND"," "," ","RESEARCH"," "," ","KASIA KITTEN"," "," ","STORYLINE"," "," ","DAVE DONKEY"};
	static string pageFive[];// = {"DISTRIBUTION"," "," ","RICHARD EMU"," "," ","DIALOGUE"," "," ","SARAH SHEEP"," "," ","CREDITS"," "," ","KEVIN KIPPER"};

	int *temp;//[];
	//private int black[];

	 bool init;

public:
	class pageStrings
	{
		int lines;
		int maxCharacters;
		string *page;

		pageStrings(string *strings) { reset(strings); }

		void reset(string *strings)
		{
			/*
			lines = strings.length;
			page = new String[lines];

			maxCharacters = 0;			
			for(int i=0;i<lines;i++)
			{			
				page[i] = new String(strings[i]);			
				if(page.length>maxCharacters) maxCharacters = page.length;			
			}
			*/			
		}
	};

private:
	//pageStrings[] pages = {new pageStrings(pageOne),new pageStrings(pageTwo),new pageStrings(pageThree),new pageStrings(pageFour),new pageStrings(pageFive)};

public:
	credits()
	{
		makeNull();
		reset();
	}
	~credits() { cleanup(); }

	bool initalised() { return init; }
	void reset();
	
private:
	void clear();
	void drawPage(int pageNum);

public:
	bool update();

	void draw(legacy::graphics g);

protected:
	void makeNull();
	void cleanup();
};

#endif