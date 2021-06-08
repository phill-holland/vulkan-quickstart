#include "legacy_canvas.h"
#include "globals.h"

int legacy::canvas::mode;

void legacy::canvas::reset()
{
    mode = MENU;

/*
    globals::Credits.reset();
    mainMenu = new menu();
    mainLevelEngine = new levelEngine();
    */
}

void legacy::canvas::run()
{/*
    // need array of average speed (5) which stores past values of how long it took ..
    while(running)
    {
        if(pause==false)
        {
            long startmillis = System.currentTimeMillis();

            if(mainMenu.update()==true)
            {								
                if (mainLevelEngine.update()==true) { mode = MENU; mainMenu.reset(); mainLevelEngine.reset(0); }
                    else { mode = GAME; mainLevelEngine.draw(screen); flushGraphics(); }

            } else { mode = MENU; mainMenu.draw(screen); flushGraphics(); }

            long totalmillis = System.currentTimeMillis() - startmillis;
    
            if(totalmillis<idealExecute)
            {		
                try { Thread.sleep(idealExecute-totalmillis); } catch(InterruptedException e) { }
            }
        } else { try { Thread.sleep(500); } catch(InterruptedException e) { } }
    }	
    */		
}

void legacy::canvas::start()
{
    _running = true;
}

void legacy::canvas::stop()
{
    _running = false;
    _pause = false;
}

void legacy::canvas::pause()
{
    _pause = true;
}

void legacy::canvas::keyPressed(int keyCode)
{
    /*
    int e = getGameAction(keyCode);

    if(mode==MENU)
    {
        mainMenu.keyPressed(e);
    }
    else if(mode==GAME)
    {
        mainLevelEngine.keyPressed(e);
    }
    */
}

void legacy::canvas::keyReleased(int keyCode)
{
    /*
    int e = getGameAction(keyCode);		

    if(mode==MENU)
    {
        mainMenu.keyReleased(e);
    }
    else if(mode==GAME)
    {
        mainLevelEngine.keyReleased(e);
    }
    */
}