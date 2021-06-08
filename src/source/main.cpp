#include <chrono>
#include <thread>
#include "canvas.h"
#include "image.h"
#include "pstring.h"
#include "plasma.h"
#include "cube.h"

/*
TODO;

1) create openGL cube
2) create generic layer for 3D objects, defining and rendering
3) openGL shader for cube
   i) wireframe (cell shading) blue neon glow (allow for different colours)
4) effects for cube
   i) pulse
   ii) rotate and shrink
   iii) pinch
   vi) mad spin, clockwise, counter
   use spline for speed up and speed down control of effects
5) remove level?.h files
6) create level and levels class, that loads from JSON
   level loader,  for different scales of screen (positions defined in %)
7) background thread for levels class,  which pre-loads next level, when timer nearly
   run-down
8) render levels (for different scales), in white wireframe skeleton, which fades to transparent
   depending on Z distance from camera
9) background level get's closer until level counter hits zero, and then
   transistions to level,  during slow distance zoom in of level, left or right
   key as player moves,  changes paralax perspective of background level

9) SDL_mixer import
 
10) assign drum loops, sounds, etc.. to each different coloured cube
    cube with hit "base" just at point sample is supposed to play -- volume gets
	louder when music and timing improves

11) create configuration class, remove "globals" static class
12) include "gravity" vector direction in level definition, falling direction

13) include alien ship at top, like space invaders, it shots the lowest cube
column,  plays sample,  changes gates,  messes up timing until you shoot it

14) make level designer

15) import error classes
16) import directX 

17) 3d play area,  each side of a cube has "gates" on it
    rotate cube (or other 3d shape) to do other levels
*/

/*
TEST JPEG LOADER
MAKE NICE CUBE, NEON GLOWING WITH NICE EFFECTS
(pulsate, spin and scale down,  using spline for speed ramp up/down)
TURN LEVELS INTO JSON LOADER
IMPORT OPENGL STUFF
removes lives
level transistions (add gates, remove gates, smooth tgransitions)
/*
import font class
import openGL

import jpeg loader

level transistions (zooming in) -- seamless ????

scaling screen

basic neon cube thing

alien spacecraft, flies above, shots cube, changes gates

turn levels into JSON files
*/
/*


package owd;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.lcdui.game.GameCanvas;

public class main extends MIDlet implements CommandListener
{
	private static mainCanvas canvas;
	private Command exitCmd = new Command("Exit",Command.EXIT,99);

	public main()
	{
		canvas = new mainCanvas(Display.getDisplay(this));
		canvas.setFullScreenMode(true);

		globals.maxWidth = canvas.getWidth();
		globals.maxHeight = canvas.getHeight();

		canvas.reset();

		canvas.addCommand(exitCmd);
		canvas.setCommandListener(this);
	}

	public void startApp()
	{
		canvas.start();
		canvas.run();
		destroyApp(false);
		notifyDestroyed();
	}

	public void destroyApp(boolean e)
	{
		canvas.stop();
	}

	public void pauseApp()
	{
		canvas.pause();
	}

	public void commandAction(Command c,Displayable s)
	{
		if(c==exitCmd)
		{
			destroyApp(false);
			notifyDestroyed();
		}
	}
 
}

*/

void Run()//long type, bool override, long device)
{
	//configuration::configuration config;
	
	//if (config.load(CONFIGURATION))
	//{		
		canvas *c = new canvas();
		if(c == NULL) return;
		if(!c->initalised()) return;

		if(!c->start()) return;

		image *i = image::createImage(string("images/test.jpg"));
		if(i == NULL) return;
		if(!i->initalised()) return;

		plasma *p = new plasma(6,40,163,36);
		if(p == NULL) return;

		cube *square = new cube();
		if(square == NULL) return;
		if(!square->initalised()) return;

		c->render(2,2,i);

		bool started = true;
		if (started)
		{
#ifdef WIN32
				MSG message;
				while (GetMessage(&message, NULL, 0, 0) > 0)
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				};
#elif defined __linux__
				while (1)
				{
					p->update();
					c->render(0,0,p);
					square->render();
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				};
#endif
		}

		if(square != NULL) delete square;
		if(p != NULL) delete p;
		if(i != NULL) delete i;
		if(c != NULL) delete c;
}

void test()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

	std::chrono::milliseconds milliseconds = std::chrono::milliseconds(2000);
	std::this_thread::sleep_for(milliseconds);

	std::chrono::high_resolution_clock::time_point interval = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - interval);

	double temp = time_span.count();
}

int main(int argc, char *argv[])
{
	//fmt::print("Hello, {}!","world");
	Run();
	
	return 0;
}
