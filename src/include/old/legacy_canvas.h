#include "legacy_graphics.h"
#include "menu.h"
#include "engine.h"

#ifndef _LEGACY_CANVAS
#define _LEGACY_CANVAS

namespace legacy
{
	class canvas// extends GameCanvas
	{
		bool _running;// = false;
		bool _pause;// = false;

		static legacy::graphics screen;

		const static long idealExecute = 30;

		menu *mainMenu;

		const static int MENU = 0;
		const static int GAME = 1;

		static int mode;

	public:	
		engine *mainLevelEngine;

	public:
		canvas()//Display d)	
		{	
			/*
			super(false);
			d.setCurrent(this);
			screen = getGraphics();
			*/
			_running = false;
			_pause = false;
		}

		void reset();
		
		void run();

		void start();
		void stop();
		void pause();

		void keyPressed(int keyCode);
		void keyReleased(int keyCode);
	};
};

#endif